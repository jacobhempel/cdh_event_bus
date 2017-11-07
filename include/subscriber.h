#ifndef INCLUDE_SUBSCRIBER_H
#define INCLUDE_SUBSCRIBER_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include <unordered_map>
#include <condition_variable>
#include <memory>
#include <iostream>

#include "tenticle.h"

std::mutex topic_wait_lock;
std::unique_lock<std::mutex> lk(topic_wait_lock);
static std::unordered_map<std::string,
    std::unique_ptr<std::condition_variable>> topic_wait;

template <typename T>
class subscriber : public tenticle {
public:
    subscriber(std::string _name, key_t message_key):
        tenticle(message_key), name(_name) {

        topic_id = getId();

        write(topic_id, "4 "+std::to_string(sizeof(T))+" "+name);

        // std::pair<long, std::string> responce;
        // responce = read(topic_id);
        //
        // std::istringsttop_wait_lockream iss(responce.second);
        // std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
        //     std::istream_iterator<std::string>{}};
        // if ((semid = semget(std::stoi(tokens[1]), 2, 0600)) < 0) {
        //     throw std::system_error(
        //         errno,
        //         std::generic_category(),
        //         "Unable to get r/w sem"
        //     );
        // }
        //
        // rw = (shm_object*)(shared_data + std::stoi(tokens[0]));
        // data_ptr = (T*)(rw + 1);



        topic_wait_lock.lock();

        std::cout << "here" << std::endl;

        topic_wait.emplace(name, std::unique_ptr<std::condition_variable>(
            new std::condition_variable));
        topic_wait_lock.unlock();
    }

    T get_value() {
        std::pair<long, std::string> data;
        data = read(topic_id);
        T return_value;

        lk.lock();
        topic_wait.find(name)->second->wait(lk);
        lk.unlock();

        // This needs to be implemented safer
        if (p(semid, 3) < 0)
            exit(1);
        if (p(semid, 0) < 0)
            exit(1);
        rw->rw_array[0] += 1;
        if (rw->rw_array[0] == 1)
            if (p(semid, 2) < 0)
                exit(1);
        v(semid, 0);
        v(semid, 3);

        return_value = *data_ptr;

        if(p(semid, 0) < 0)
            exit(1);
        rw->rw_array[0] -= 1;
        if (rw->rw_array[0] == 0)
            if (v(semid, 2) < 0)
                exit(1);
        v(semid, 0);

        return return_value;
    }

private:
    long topic_id;
    std::string name;
    int semid;
    shm_object* rw;
    T* data_ptr;
};

void* listen_for_event(void* data) {
    tenticle t(*static_cast<key_t*>(data));
    while (1) {
        std::pair<long, std::string> event = t.read(3);
        auto item = topic_wait.find(event.second);
        if (item != topic_wait.end()) {
            item->second->notify_all();
        }
    }
}

#endif  // INCLUDE_SUBSCRIBER_H
