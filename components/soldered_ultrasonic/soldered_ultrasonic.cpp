/**
 * @file soldered_ultrasonic.cpp
 * @brief Implementation of the soldered_ultrasonic ESPHome component
 * @author Soldered Electronics
 */

#include "soldered_ultrasonic.h"

#include <cmath>

#include "esphome/core/log.h"

namespace esphome {
namespace soldered_ultrasonic {

static const char *const TAG = "soldered_ultrasonic";

static const uint8_t REG_TAKE_MEASUREMENT = 0x00;
static const uint8_t REG_DURATION = 0x02;

/// The ATtiny runs the whole HC-SR04 measurement (pulseIn() with a 38 ms timeout) inside its I2C receive handler and
/// does not answer on I2C until it is done, so the result is read only after this delay.
static const uint32_t MEASUREMENT_TIME_MS = 50;

static const float SPEED_OF_SOUND_M_PER_S = 343.0f;

void SolderedUltrasonicSensor::setup() {
  // There is no ID register; reading the last echo time (without triggering a measurement) checks the board answers
  uint16_t duration;
  if (!this->read_u16_(REG_DURATION, &duration)) {
    ESP_LOGE(TAG, "Board not responding");
    this->mark_failed();
  }
}

void SolderedUltrasonicSensor::update() {
  if (this->measuring_) {
    ESP_LOGD(TAG, "'%s': previous measurement still running, skipping", this->get_name().c_str());
    return;
  }
  if (!this->select_register_(REG_TAKE_MEASUREMENT)) {
    ESP_LOGW(TAG, "'%s': triggering measurement failed", this->get_name().c_str());
    this->status_set_warning();
    return;
  }
  this->measuring_ = true;
  this->set_timeout("read", MEASUREMENT_TIME_MS, [this]() { this->read_result_(); });
}

void SolderedUltrasonicSensor::read_result_() {
  this->measuring_ = false;

  uint16_t duration_us;
  if (!this->read_u16_(REG_DURATION, &duration_us)) {
    ESP_LOGW(TAG, "'%s': reading echo time failed", this->get_name().c_str());
    this->status_set_warning();
    return;
  }
  this->status_clear_warning();

  // pulseIn() on the ATtiny returns 0 when no echo arrived within 38 ms (nothing in range)
  if (duration_us == 0) {
    ESP_LOGD(TAG, "'%s': no echo", this->get_name().c_str());
    this->publish_state(NAN);
    return;
  }

  float distance = duration_us * 1e-6f * SPEED_OF_SOUND_M_PER_S / 2.0f;
  ESP_LOGD(TAG, "'%s': echo=%u us, distance=%.3f m", this->get_name().c_str(), duration_us, distance);
  this->publish_state(distance);
}

void SolderedUltrasonicSensor::dump_config() {
  LOG_SENSOR("", "Soldered Ultrasonic Sensor", this);
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, ESP_LOG_MSG_COMM_FAIL);
  }
  LOG_UPDATE_INTERVAL(this);
}

bool SolderedUltrasonicSensor::select_register_(uint8_t reg) { return this->write(&reg, 1) == i2c::ERROR_OK; }

bool SolderedUltrasonicSensor::read_u16_(uint8_t reg, uint16_t *value) {
  // Separate write and read transactions instead of a repeated start, matching the easyC Arduino library
  if (!this->select_register_(reg))
    return false;
  uint8_t raw[2];
  if (this->read(raw, sizeof(raw)) != i2c::ERROR_OK)
    return false;
  *value = uint16_t(raw[0]) | (uint16_t(raw[1]) << 8);  // little-endian
  return true;
}

}  // namespace soldered_ultrasonic
}  // namespace esphome
