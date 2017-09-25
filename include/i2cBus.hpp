#ifndef _I2C_BUS_HPP
#define _I2C_BUS_HPP

#include "octopOS.hpp"
#include "publisher.hpp"
#include "ACS.hpp"
#include "spacehauc-i2c-mock.h"

using namespace spacehauc_i2c_mock;

class i2cBus : public publisher<ACS_DATA_TYPE> {
 private:
  ACS_DATA_TYPE ACS_i2c_data;

 public:
  void InternalThreadEntry();
  void publish();
  // ~i2cBus() = default;

};

void i2cBus::InternalThreadEntry() {
  int bus = 1;

  I2C_Bus::init(bus);

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
    for (auto aSub : aTopic->mySubs) {
      aSub->onEvent(ACS_i2c_data);
    }
  }
}

#endif  // _I2C_BUS_HPP
