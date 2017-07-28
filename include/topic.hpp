#ifndef TOPIC_H
#define TOPIC_H

#include "subscriber.hpp"

template <class T>
class topic {
 public:
  void addSubcriber(subscriber* newSub);
  vector<subscriber*> mySubs;
};

void topic::addSubcriber(subscriber* newSub) {
  mySubs.push_back(newSub);
}

#endif   // TOPIC_H
