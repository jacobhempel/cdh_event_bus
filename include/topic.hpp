#ifndef TOPIC_H
#define TOPIC_H

#include "subscriber.hpp"

template <class T>
class topic {
 public:
  void addSubcriber(subscriber<T>* newSub) {
    mySubs.push_back(newSub);
  };

  vector<subscriber<T>*> mySubs;
};

#endif   // TOPIC_H
