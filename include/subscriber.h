#include "tenticle.h"

template <typename T>
class subscriber : public tenticle {
public:
  typedef void (* callback)(void) callback;
  subscriber(std::string topic_name, key_t _shared_memory)
    : tenticle(shared_memory),
      topic(topic_name),
      shared_memory(_shared_memory) {
    subscriber::register(new_data, topic_name, shared_memory);
  }

  // Read the data queue if there's anything there
  // Blocks if no data available
  std::string get_data();

  // Callback: push the new data onto this subscriber's data queue
  #define TYPE ???
  void new_data() {
    dataQ.enqueue(read(TYPE).second);
  }

private:
  std::string topic;
  std::queue<std::string> dataQ;
  key_t shared_memory;

  // Registers CB to TOPIC; CB will be invoked when new messages are available
  // on TOPIC
  static bool register(callback cb, std::string topic, key_t shared_memory);

  static void wait_for_data();

  // A map from topic to the memory for that topic
  static std::map<std::string, key_t> topic_memory = \
    std::map<std::string, key_t>();
  
  // A map from topics to registered callbacks
  static std::map<std::string, std::list<callback> > registered_callbacks = \
    std::map<std::string, std::list<callback> >();
}
