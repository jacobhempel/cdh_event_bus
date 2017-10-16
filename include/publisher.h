#ifndef INCLUDE_PUBLISHER_H
#define INCLUDE_PUBLISHER_H

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include <iostream>
#include <unistd.h>

#include "tenticle.h"

template <typename T>
class publisher : public tenticle {
public:
    publisher(std::string _name, key_t message_key):
        tenticle(message_key), name(_name) {

        write(1, std::to_string(sizeof(T))+" "+name);

        std::pair<long, std::string> fuck;
        fuck = read(2);

        std::istringstream iss(fuck.second);
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
    }

    bool publish(T data) {
        bool return_value = false;
        if (p(semid, 1) >= 0) {
            rw->rw_array[1] += 1;
            if (rw->rw_array[1] == 1) {
                p(semid, 0);  // Should check for success
            }

            v(semid, 1);
        }

        if (p(semid, 2) >= 0) {
            *data_ptr = data;
            return_value = true;
            std::cout << data_ptr << " / " << *data_ptr << std::endl;
            write(3, name);
            v(semid, 2);
        }

        if (p(semid, 1) >= 0) {
            rw->rw_array[1] -= 1;
            if (rw->rw_array[1] == 0)
                v(semid, 0);  // Should check for success
            v(semid, 1);
        }

        return return_value;
    }
private:
    std::string name;
    int semid;
    shm_object* rw;
    T* data_ptr;
};

#endif  // INCLUDE_PUBLISHER_H
