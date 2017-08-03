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
  void onEvent(void *newData);
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
      fresh = 0;
    }
}

void ACS::onEvent(void* incomingData) {
  newData.x = incomingData->x;
  newData.y = incomingData->y;
  newData.z = incomingData->z;
}
