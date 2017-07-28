#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include "octopOS.hpp"

template <class T>
class subscriber : public SPACEHAUC_thread {
 private:
 public:
  virtual void onEvent(T eventData);
};

#endif   // SUBSCRIBER_H
