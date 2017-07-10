#include <iostream>
#include "../include/octopOS.hpp"

void eventBusTopic::addSubcriber(subscriber* newSub) {
  subList.push_back(*newSub);
}

void eventBusTopic::notify(int data) {
  for (unsigned int i = 0; i < subList.size(); i++) {
    subList[i].onEvent(data);
  }
}

void eventBusTopic::setData(int input) {
  data = input;
}

void publisher::publish(eventBusTopic* topic) {
  topic->setData(randomData);
  std::cout << "publisher published" << randomData << std:: endl;
  topic->notify(randomData);
}

void subscriber::onEvent(int data) {
  std::cout << "onEvent() says " << data << std::endl;
}
