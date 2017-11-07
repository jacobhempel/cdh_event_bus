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
#include <sstream>
#include <algorithm>
#include <iterator>

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
    static void* listen_for_child(void* msgkey);

    bool propagate_to_subscribers(std::string name);
    long subscribe_to_topic(std::string name,
        unsigned tenticle, long message_id, long size = -1);

    static void sig_handler(int sig);
private:
    static int shmid, tenticle_ids[NUMMODULES];
    static tenticle* tenticles[NUMMODULES];
    static std::vector<int> semids;
    intptr_t *shared_ptr, *shared_end_ptr;

  // topic_data: map[topic name, TopicInfo]
  // TopicInfo := (offset into shared mem,
  //               reference to shared mem (also a pointer),
  //               array of subscribers)
    std::map<std::string,
	           std::tuple<unsigned,
			                  key_t,
			                  std::vector<unsigned>>> topic_data;
    std::mutex topic_data_rdlock,
        topic_data_wrlock;
    unsigned topic_data_readers = 0;

    static std::mutex topic_data_rdlock, topic_data_rdtry,
        topic_data_wrlock, topic_data_lock;
    static unsigned topic_data_readers, topic_data_writers;

    void topic_reader_in();
    void topic_reader_out();
    void topic_writer_in();
    void topic_writer_out();

    octopOS();
};

#endif  // INCLUDE_OCTOPOS_H
