// #include "tenticle.h"
//
// template <typename T>
// class subscriber : public tenticle {
// public:
//   extern void callback (*callback)(void);
//   subscriber(std::string topic_name, key_t _shared_memory)
//     : tenticle(shared_memory),
//       topic(topic_name),
//       shared_memory(_shared_memory) {
//     subscriber::register(new_data, topic_name, shared_memory);
//     topic_ids_lock.lock();
//     if (topic_ids.count(topic_name) == 0) {
//       topic_ids[topic_name] = (getpid() << 31) + node_count++;
//     }
//     topic_ids_lock.unlock();
//   }
//
//   // Read the data queue if there's anything there
//   // Blocks if no data available
//   std::string get_data();
//
//   // Callback: push the new data onto this subscriber's data queue
//   void new_data() {
//     dataQ.enqueue(read(id).second);
//   }
//
// private:
//   std::string topic;
//   std::queue<std::string> dataQ;
//   key_t shared_memory;
//   long id;
//
//   static unsigned node_count;
//   std::mutex topic_ids_lock;
//
//   // Registers CB to TOPIC; CB will be invoked when new messages are available
//   // on TOPIC
//   static bool register(callback cb, std::string topic, key_t shared_memory);
//
//   static void wait_for_data();
//
//   // A map from topic to the memory for that topic
//   static std::map<std::string, key_t> topic_memory =
//     std::map<std::string, key_t>();
//
//   static std::map<std::string, long> topic_ids =
//     std::map<std::string, long>();
//
//   // A map from topics to registered callbacks
//   static std::map<std::string, std::list<callback> > registered_callbacks =
//     std::map<std::string, std::list<callback> >();
// }
