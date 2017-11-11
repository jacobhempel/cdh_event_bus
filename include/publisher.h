#ifndef INCLUDE_PUBLISHER_H
#define INCLUDE_PUBLISHER_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>

#include "tenticle.h"

#include <iostream>

template <typename T>
class publisher : public tenticle {
public:
    publisher(std::string _name, key_t message_key):
        tenticle(message_key), name(_name) {
        id = getTempId(tenticle::role_t::PUBLISHER);

        write(CREATE_PUB, std::to_string(id)+" "+std::to_string(sizeof(T))+" "+name);

        std::pair<long, std::string> response;
        response = read(id);

        std::istringstream iss(response.second);
        std::vector<std::string> tokens{std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>{}};
        if ((semid = semget(std::stoi(tokens[1]), 2, 0600)) < 0) {
            throw std::system_error(
                errno,
                std::generic_category(),
                "Unable to get r/w sem"
            );
        }

        rw = (shm_object*)(shared_data + std::stoi(tokens[0]));

        data_ptr = (T*)(rw + 1);
        id = std::stoi(tokens[2]);
    }

    bool publish(T data) {
        bool return_value = false;
        if (p(semid, 1) >= 0) {
            rw->rw_array[1] += 1;
            if (rw->rw_array[1] == 1) {
                p(semid, 3);  // Should check for success
            }

            v(semid, 1);
        }

        if (p(semid, 2) >= 0) {
            *data_ptr = data;
            return_value = true;
            write(PUBLISH_CODE, name);
            v(semid, 2);
        }

        if (p(semid, 1) >= 0) {
            rw->rw_array[1] -= 1;
            if (rw->rw_array[1] == 0)
                v(semid, 3);  // Should check for success
            v(semid, 1);
        }
        return return_value;
    }
private:
    long id;
    std::string name;
    int semid;
    shm_object* rw;
    T* data_ptr;
};

#endif  // INCLUDE_PUBLISHER_H
