#include "tentacle.h"

tentacle::tentacle(key_t msg_key) {
    if ((message_que = msgget(msg_key, 0600)) < 0 ) {
        throw std::system_error(
            errno,
            std::generic_category(),
            "Unable to set signal handler"
        );
    }
    if (shared_data == NULL)
        if (!(shared_data = connect_shm())) {
            throw std::system_error(
                errno,
                std::generic_category(),
                "Unable to connect tentacle to shm_object"
            );
        }
}

std::pair<long, std::string> tentacle::read(long type, bool block, bool under) {
    message_buffer my_buffer;
    std::pair<long, std::string> return_value(0, "");
    int rc;

    type *= (under)? -1 : 1;

    if (!block) {
        rc = msgrcv(message_que, &my_buffer,
            sizeof(my_buffer.text), type, IPC_NOWAIT); // ADD THE ONLY UNDER STUFF HERE
    }
    else
        rc = msgrcv(message_que, &my_buffer,
            sizeof(my_buffer.text), type, 0); // AND HERE TOO PLZ

    if (rc >= 0) {
        return_value.first = my_buffer.type;
        return_value.second = my_buffer.text;
        // std::cout << "I'm RC: " << my_buffer.type << std::endl;
    }
    return return_value;
}

bool tentacle::write(long type, std::string data) {
    message_buffer my_buffer;
    bool return_value = false;

    my_buffer.type = type;
    strncpy(my_buffer.text, data.c_str(), data.size());
    my_buffer.text[data.size()] = '\0';

    int rc = msgsnd(message_que, &my_buffer,
        sizeof(my_buffer.text), IPC_NOWAIT);

    if (rc >= 0) {
        return_value = true;
    }

    return return_value;
}

bool tentacle::write(std::pair<long, std::string> pair) {
    return write(pair.first, pair.second);
}

long tentacle::getTempId(role_t role) {
    long return_value = 0;
    static ushort rand_seed[3] = { 0 };

    switch (role) {
      default:
        break;
      case SUBSCRIBER:
        return_value |= SUB_BIT;
        break;
    }
    return_value |= TEMP_BIT;

    id_lock.lock();
    if (!rand_seed[0])
        initRand(rand_seed);

    return_value |= nrand48(rand_seed) & 0x1fffffff;
    id_lock.unlock();

    return return_value;
}

void tentacle::initRand(ushort *rand_seed) {
    struct timeval rand_time;
    gettimeofday(&rand_time, (struct timezone*)NULL);

    rand_seed[0] = (ushort) rand_time.tv_sec;
    rand_seed[1] = (ushort) (rand_time.tv_usec >> 16);
    rand_seed[2] = (getpid());

    return; // NOT SURE IF WE WANT LATER : rand_seed;
}

std::mutex tentacle::id_lock;
int tentacle::id = 0;
intptr_t* tentacle::shared_data = NULL;
int tentacle::message_que;
