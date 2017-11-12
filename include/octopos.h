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
#include <memory>

#include "utility.h"
#include "tentacle.h"

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
    std::pair<unsigned, key_t> subscribe_to_topic(std::string name,
        unsigned tentacle, octopOS_id_t subscriber_id, long size = -1);

    static void sig_handler(int sig);
private:
    static long get_id(tentacle::role_t role);

    static int shmid, tentacle_ids[NUMMODULES];
    static std::vector<std::shared_ptr<tentacle>> tentacles;
    static std::vector<int> semids;
    static intptr_t *shared_ptr, *shared_end_ptr;

  // topic_data: map[topic name, TopicInfo]
  // TopicInfo := (offset into shared mem,
  //               reference to shared mem (also a pointer) [ I think it is the semkey],
  //               array of subscribers
  //                 1. topic ID
  //                 2. tentacle ID)
    static std::map<std::string,
	           std::tuple<unsigned,
			                  key_t,
			                  std::vector<std::pair<octopOS_id_t, unsigned>>
                        >
                   > topic_data;

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
