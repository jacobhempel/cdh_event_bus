#ifndef INCLUDE_TENTICLE_H
#define INCLUDE_TENTICLE_H

#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <string>
#include <utility>
#include <mutex>

#include "utility.h"

class tenticle {
    friend class octopOS;
private:
    static int id;
    static std::mutex id_lock;
protected:
    int message_que, sem_index;
    static intptr_t* shared_data;
public:
    tenticle(key_t msg_key);
    std::pair<long, std::string> read(long type, bool ignore = false);
    bool write(long type, std::string data);
    bool write(std::pair<long, std::string> pair);
    long getId();
};

#endif  // INCLUDE_TENTICLE_H
