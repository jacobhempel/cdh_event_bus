#ifndef _PUBLISHER_HPP
#define _PUBLISHER_HPP

#include "octopOS.hpp"
#include "topic.hpp"

template <class T>
class publisher : public SPACEHAUC_thread {
 protected:
  vector<topic<T>*> myTopics;
  T data;
 public:
  publisher(topic<T>* firstTopic);
  // virtual ~publisher() = default;
  void addTopic(topic<T>* newTopic);
  virtual void publish() = 0;

};

#endif  // _PUBLISHER_HPP
