#ifndef INCLUDE_TENTICLE_H
#define INCLUDE_TENTICLE_H

#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>
#include <string>
#include <utility>
#include <mutex>

#include "utility.h"

class tenticle {
    friend class octopOS;
private:
    static int id;
    static std::mutex id_lock;
    static void initRand(ushort* rand_seed);
protected:
    int message_que, sem_index;
    static intptr_t* shared_data;
    enum role_t {SUBSCRIBER, PUBLISHER }; // PROTECT ME SQUIRE
    long getTempId(role_t role);
public:
    tenticle(key_t msg_key);
    std::pair<long, std::string> read(long type, bool block = true);
    bool write(long type, std::string data);
    bool write(std::pair<long, std::string> pair);
};

#endif  // INCLUDE_TENTICLE_H
