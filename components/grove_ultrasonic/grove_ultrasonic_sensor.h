#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace grove_ultrasonic {

class GroveUltrasonicSensor : public PollingComponent, public sensor::Sensor {
 public:
  void set_pin(GPIOPin *pin) { pin_ = pin; }

  void setup() override;
  void update() override;

 protected:
  GPIOPin *pin_{nullptr};
};

}  // namespace grove_ultrasonic
}  // namespace esphome

