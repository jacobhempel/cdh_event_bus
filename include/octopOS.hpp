#include <vector>
#include <iostream>

using std::vector;

class subscriber {
public:
  void onEvent(int data);
};

class eventBusTopic {
private:
  int data;
  vector<subscriber> subList;
public:
  void addSubcriber(subscriber* newSub);
  void notify(int data);
  void setData(int input);
};

class publisher {
public:
  void publish(eventBusTopic* topic);
  int randomData;
};
