// #include "subscriber.h"
//
// std::string subscriber::get_data() {
//   while (dataQ.empty()) {
//     sleep(SLEEP_TIME);
//   }
//   return dataQ.dequeue();
// }
//
// bool subscriber::register(callback cb, std::string topic) {
//   auto callbacks = registered_callbacks[topic]; // ITS PROBABLY MY FAULT
//   callbacks.push_back(cb);
// }
//
// void subscriber::wait_for_data() {
//   while (true) {
//     for (auto topic_callbacks : registered_callbacks) {
//       auto topic = topic_callbacks.key;
//       auto callbacks = topic_callbacks.value
//       key_t memory = topic_memory[topic];
//       std::pair<bool, std::string> message = read(topic_ids[topic], true);
//       if (message.first) {
//       	for (auto cb: callbacks) {
//       	  //cb(message.second);
//       	}
//       }
//     }
//   }
// }
