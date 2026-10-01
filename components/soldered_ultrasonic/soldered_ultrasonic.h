/**
 * @file soldered_ultrasonic.h
 * @brief Public API for the soldered_ultrasonic ESPHome component
 * @author Soldered Electronics
 *
 * Driver for the Soldered Ultrasonic Sensor with Qwiic (HC-SR04 + ATtiny404), ported from the Soldered Ultrasonic
 * Sensor easyC Arduino library. The ATtiny runs the HC-SR04 measurement itself when register 0 is written and keeps
 * the last echo pulse width (us) and distance (cm) in registers 2 and 1. This component triggers a measurement on
 * every update, reads the echo pulse width once the ATtiny is done and converts it to meters at 343 m/s, the same
 * way the built-in ultrasonic component does.
 */

#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace soldered_ultrasonic {

class SolderedUltrasonicSensor : public sensor::Sensor, public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  /// Write a register address on its own transaction (with STOP), like the easyC Arduino library does
  bool select_register_(uint8_t reg);
  bool read_u16_(uint8_t reg, uint16_t *value);
  void read_result_();

  bool measuring_{false};
};

}  // namespace soldered_ultrasonic
}  // namespace esphome
