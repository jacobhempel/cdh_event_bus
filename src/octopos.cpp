#include "octopos.h"

void octopOS::sig_handler(int sig) {
    if (shmctl(shmid, IPC_RMID, 0) < 0) {
        if (sig < 0)
            throw std::system_error(
                errno,
                std::generic_category(),
                "Unable to remove shared memory segment"
            );
    }
    for (auto i : semids) {
        if(semctl(i, 0, IPC_RMID) < 0) {
            if (sig < 0)
                throw std::system_error(
                    errno,
                    std::generic_category(),
                    "Unable to remove semaphores"
                );
        }
    }
    for (unsigned i = 0; i < NUMMODULES; ++i) {
        if (msgctl(tenticles[i], IPC_RMID, NULL) < 0) {
            if (sig < 0)
                throw std::system_error(
                    errno,
                    std::generic_category(),
                    "Unable to remove tenticles"
                );
        }
    }
}
// initialize the thing
octopOS::octopOS() {
    int signals[] = {SIGHUP, SIGINT, SIGQUIT, SIGBUS, SIGTERM, SIGSEGV, SIGFPE};
    unsigned num_signals = sizeof(signals) / sizeof(int);
    sigset_t signal_mask;
    struct sigaction signal_action;

    sigemptyset(&signal_mask);
    for (unsigned i = 0; i < num_signals; ++i)
        sigaddset(&signal_mask, signals[i]);
    for (unsigned i = 0; i < num_signals; ++i) {
        signal_action.sa_handler = sig_handler;
        signal_action.sa_mask    = signal_mask;
        signal_action.sa_flags   = 0;
        if (sigaction(signals[i], &signal_action, NULL) < 0)
            throw std::system_error(
                errno,
                std::generic_category(),
                "Unable to set signal handler"
            );
    }

    if ((shmid = shmget(MEMKEY, SHMSIZE, IPC_CREAT | 0600)) < 0) {
        throw std::system_error(
            errno,
            std::generic_category(),
            "Unable to create shared memory segment"
        );
    }

    if ((shared_ptr = (intptr_t*)shmat(shmid, NULL, 0)) < (intptr_t*)0) {
        std::cerr << "Unable to attach shared memory segment: "
            << errno << std::endl;
        sig_handler(-1);
    }

    tenticle::shared_data = shared_ptr;

    for (unsigned i; i < NUMMODULES; ++i) {
        if ((tenticles[i] = msgget(MSGKEY + i, IPC_CREAT | 0600)) < 0) {
            throw std::system_error(
                errno,
                std::generic_category(),
                "Unable to create message queues"
            );
        }
    }

    shared_end_ptr = shared_ptr;
}

std::pair<unsigned, key_t> octopOS::create_new_topic
    (std::string name, unsigned size) {
    std::pair<unsigned, key_t> return_value(0, 0);

    topic_data_wrlock.lock();
    auto tmp = topic_data.find(name);
    if (tmp == topic_data.end()) {
        key_t my_sem;
        if ((my_sem = semget(SEMKEY+semids.size(), 3, IPC_CREAT | 0600)) >= 0) {
            semsetall(my_sem, 3, 1);

            topic_data.emplace(name, std::tuple<unsigned, key_t,
                std::vector<unsigned>>(
                    shared_end_ptr - shared_ptr,
                    SEMKEY+semids.size(),
                    std::vector<unsigned>())).second;

                return_value.first = shared_end_ptr - shared_ptr;
                return_value.second = SEMKEY+semids.size();
                semids.push_back(my_sem);
                shm_object* init = (shm_object*)shared_end_ptr;
                init->rw_array[0] = init->rw_array[1] = 0;
                shared_end_ptr +=
                    (size + sizeof(shm_object)) / sizeof(intptr_t) + 1;  // This size calculation has extra padding
        }
    } else {
        return_value.first = std::get<0>(tmp->second);
        return_value.second = std::get<1>(tmp->second);
    }
    topic_data_wrlock.unlock();

    return return_value;
}

bool octopOS::propagate_to_subscribers(std::string name) {
    bool return_value = false;

    topic_reader_in();

    auto tmp = topic_data.find(name);
    if (tmp != topic_data.end()) {
        for (auto i : std::get<2>(tmp->second)) {
            std::cout << "Propagating: " << name << ", " << i << std::endl;
            std::cout << "\tData: " << shared_ptr[1] << std::endl;
            // Notify each subscriber
        }
        std::cout << shared_ptr << " / " << shared_end_ptr << std::endl;
        return_value = true;
    }

    topic_reader_out();

    return return_value;
}

unsigned octopOS::subscribe_to_topic(std::string name, unsigned tenticle) {
    unsigned return_value = 0;

    topic_data_wrlock.lock();

    auto tmp = topic_data.find(name);
    if (tmp != topic_data.end()) {
        std::get<2>(tmp->second).push_back(tenticle);
        return_value = std::get<0>(tmp->second);
    }

    topic_data_wrlock.unlock();

    return return_value;
}

void octopOS::topic_reader_in() {
    topic_data_rdlock.lock();
    if (topic_data_readers == 0)
        topic_data_wrlock.lock();
    topic_data_readers += 1;
    topic_data_rdlock.unlock();
}

void octopOS::topic_reader_out() {
    topic_data_rdlock.lock();
    topic_data_readers -= 1;
    if (topic_data_readers == 0)
        topic_data_wrlock.unlock();
    topic_data_rdlock.unlock();
}

octopOS::~octopOS() {
    try  {
        sig_handler(0);
    } catch (const std::system_error& ex) {
        std::cerr << ex.code().message() << std::endl;
        std::cerr << ex.what() << std::endl;
    }
}

int octopOS::shmid = 0;
int octopOS::tenticles[NUMMODULES];
std::vector<int> octopOS::semids;
