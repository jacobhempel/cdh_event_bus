#include "octopOS.hpp"
#include "publisher.hpp"
#include "ACS.hpp"
#include "../../SPACEHAUC-I2C-dev/include/spacehauc-i2c-mock.h"

class i2cBus : public publisher<ACS_DATA_TYPE> {
 private:
  ACS_DATA_TYPE ACS_i2c_data;

 public:

};

virtual void i2cBus::InternalThreadEntry() {
  int bus = 1;

  if (I2C_Bus::init(bus) == false) {
    cerr << "Error: I2C bus failed to open." << endl;
  }

  MCP9808 sensorX(0x18);
  MCP9808 sensorY(0x28);
  MCP9808 sensorZ(0x38);

  sensorX.init();
  sensorY.init();
  sensorZ.init();

  while (1) {
    ACS_i2c_data.x = sensorX.read();
    ACS_i2c_data.y = sensorY.read();
    ACS_i2c_data.z = sensorZ.read();
    publish();
  }
}

void i2cBus::publish() {
  for (auto aTopic : myTopics) {
    for (auto aSub : aTopic.mySubs) {
      aSub.onEvent(ACS_i2c_data);
    }
  }
}
