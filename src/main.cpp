#include <iostream>
#include "../include/octopOS.hpp"

int main(int argc, char const *argv[]) {
  publisher myPublisher;
  subscriber mySubscriber;
  eventBusTopic test;
  test.addSubcriber(&mySubscriber);

  for (int i = 0; i < 10; i++) {
    myPublisher.randomData = i;
    myPublisher.publish(&test);
  }
  return 0;
}
