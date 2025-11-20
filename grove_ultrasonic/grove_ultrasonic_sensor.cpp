#include "grove_ultrasonic_sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace grove_ultrasonic {

static const char *const TAG = "grove_ultrasonic";

void GroveUltrasonicSensor::setup() {
  ESP_LOGCONFIG(TAG, "Grove Ultrasonic sensor setup");
  if (this->pin_ != nullptr) {
    this->pin_->setup();
  }
}

void GroveUltrasonicSensor::update() {
  if (this->pin_ == nullptr) {
    ESP_LOGE(TAG, "No pin configured");
    this->publish_state(NAN);
    return;
  }

  using namespace esphome::gpio;

  // 1. Send trigger pulse (>10 µs) on SIG
  this->pin_->pin_mode(FLAG_OUTPUT);
  this->pin_->digital_write(false);
  delayMicroseconds(2);
  this->pin_->digital_write(true);
  delayMicroseconds(15);  // >10us
  this->pin_->digital_write(false);

  // 2. Switch to input and measure pulse width (manual pulseIn)
  this->pin_->pin_mode(FLAG_INPUT);

  const unsigned long timeout = 25000UL;  // ~25ms

  // Wait for signal to go HIGH
  unsigned long start = micros();
  while (!this->pin_->digital_read()) {
    if (micros() - start > timeout) {
      ESP_LOGW(TAG, "Timeout waiting for echo start");
      this->publish_state(NAN);
      return;
    }
  }

  // Measure how long it stays HIGH
  unsigned long echo_start = micros();
  while (this->pin_->digital_read()) {
    if (micros() - echo_start > timeout) {
      ESP_LOGW(TAG, "Timeout waiting for echo end");
      this->publish_state(NAN);
      return;
    }
  }
  unsigned long duration = micros() - echo_start;

  // 3. Convert µs -> cm  (speed of sound ~0.0343 cm/µs, /2 for round trip)
  float distance_cm = (duration * 0.0343f) / 2.0f;

  this->publish_state(distance_cm);
}


}  // namespace grove_ultrasonic
}  // namespace esphome

