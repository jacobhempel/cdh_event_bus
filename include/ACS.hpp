#ifndef _ACS_HPP
#define _ACS_HPP

#include "octopOS.hpp"

struct ACS_DATA_TYPE {
  double x;
  double y;
  double z;
};

class ACS : public subscriber<ACS_DATA_TYPE> {
private:
  ACS_DATA_TYPE currentData;
  ACS_DATA_TYPE newData;

  bool fresh;
  void update();

public:
  void onEvent(ACS_DATA_TYPE *newData);
  void InternalThreadEntry();
};

void ACS::InternalThreadEntry() {
  while (1) {
    cout << "ACS did stuff with " << currentData.x << ", " << currentData.y << ", " << currentData.z << endl;
    update();
  }
}

void ACS::update() {
    if (fresh) {
      currentData.x = newData.x;
      currentData.y = newData.y;
      currentData.z = newData.z;
      fresh = false;
    }
}

void ACS::onEvent(ACS_DATA_TYPE* incomingData) {
  newData.x = incomingData->x;
  newData.y = incomingData->y;
  newData.z = incomingData->z;
}

#endif  // _ACS_HPP
