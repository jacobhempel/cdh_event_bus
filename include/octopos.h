#ifndef INCLUDE_OCTOPOS_H
#define INCLUDE_OCTOPOS_H

#include <signal.h>

#include <cstdint>
#include <iostream>
#include <mutex>
#include <map>
#include <string>
#include <vector>
#include <tuple>

#include "utility.h"
#include "tenticle.h"

class octopOS {
public:
    static octopOS& getInstance() {
        static octopOS instance;
        return instance;
    }
    octopOS(octopOS const&) = delete;
    void operator=(octopOS const &) = delete;

    ~octopOS();

    std::pair<unsigned, key_t> create_new_topic
        (std::string name, unsigned size);
    bool propagate_to_subscribers(std::string name);
    unsigned subscribe_to_topic(std::string name, unsigned tenticle);

    static void sig_handler(int sig);
private:
    static int shmid, tenticles[NUMMODULES];
    static std::vector<int> semids;
    intptr_t *shared_ptr, *shared_end_ptr;

    std::map<std::string, std::tuple<unsigned, key_t,
        std::vector<unsigned>>> topic_data;
    std::mutex topic_data_rdlock,
        topic_data_wrlock;
    unsigned topic_data_readers = 0;



    void topic_reader_in();
    void topic_reader_out();

    octopOS();
};

#endif  // INCLUDE_OCTOPOS_H
