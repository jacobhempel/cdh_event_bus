#ifndef INCLUDE_TENTICLE_H
#define INCLUDE_TENTICLE_H

#include <string.h>

#include <cstdio>
#include <string>
#include <utility>

#include "utility.h"

class tenticle {
    friend class octopOS;
protected:
    int message_que, sem_index;
    static intptr_t* shared_data;
public:
    tenticle(key_t msg_key);
    std::pair<long, std::string> read(long type);
    bool write(long type, std::string data);
};

#endif  // INCLUDE_TENTICLE_H
