#ifndef INCLUDE_tentacle_H
#define INCLUDE_tentacle_H

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

class tentacle {
    friend class octopOS;
private:
    static int id;
    static std::mutex id_lock;
    static void initRand(ushort* rand_seed);
protected:
    static int message_que;
    static intptr_t* shared_data;
    enum role_t {SUBSCRIBER, PUBLISHER }; // PROTECT ME SQUIRE
    static long getTempId(role_t role);
public:
    tentacle(key_t msg_key);
    static std::pair<long, std::string> read(long type, bool block = true,
                                      bool under = false);
    bool write(long type, std::string data);
    bool write(std::pair<long, std::string> pair);
};

#endif  // INCLUDE_tentacle_H
