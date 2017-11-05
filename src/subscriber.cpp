#include "subscriber.h"

std::string subscriber::get_data() {
  while (dataQ.empty()) {
    sleep(SLEEP_TIME);
  }
  return dataQ.dequeue();
}

bool subscriber::register(callback cb, std::string topic, key_t shared_memory) {
  auto callbacks = registered_callbacks[topic];
  callbacks.push_back(cb);
  topic_memory[topic] = shared_memory;
}

void subscriber::wait_for_data() {
  while (1) {
    for (auto topic_callbacks : registered_callbacks) {
      auto topic = topic_callbacks.key;
      auto callbacks = topic_callbacks.value
      key_t memory = topic_memory[topic];

      if (new_message_in(memory)) {
	for (auto cb: callbacks) {
	  cb();
	}
      }
    }
  }
}

bool new_message_in(key_t memory) {
  
}
