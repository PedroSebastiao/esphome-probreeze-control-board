#include "esphome/core/log.h"
#include "probreeze_sensor.h"
#include <cinttypes>

namespace esphome {
namespace probreeze {

static const char *const TAG = "probreeze.sensor";

void ProBreezeSensor::setup() {
  this->parent_->register_temperature_listener([this](uint8_t temperature) {
    this->temperature_sensor_->publish_state(temperature);
  });
  this->parent_->register_humidity_listener([this](uint8_t humidity) {
    this->humidity_sensor_->publish_state(humidity);
  });
}

}  // namespace probreeze
}  // namespace esphome
