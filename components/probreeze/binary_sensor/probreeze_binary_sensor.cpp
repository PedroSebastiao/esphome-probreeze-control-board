#include "esphome/core/log.h"
#include "probreeze_binary_sensor.h"
#include <cinttypes>

namespace esphome {
namespace probreeze {

static const char *const TAG = "probreeze.binary_sensor";

void ProBreezeBinarySensor::setup() {
  this->parent_->register_tank_full_listener([this](bool tank_full) {
    this->tank_full_binary_sensor_->publish_state(tank_full);
  });
}

}  // namespace probreeze
}  // namespace esphome
