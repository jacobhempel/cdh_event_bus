#include "octopOS.hpp"
#include "topic.hpp"

template <class T>
class publisher : public SPACEHAUC_thread {
 private:
  T data;
  vector<topic*> myTopics;

 public:
  publisher(topic* firstTopic);
  addTopic(topic* newTopic);
  virtual void publish();

};
