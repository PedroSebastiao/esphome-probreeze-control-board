#include "esphome/core/log.h"
#include "probreeze_fan.h"

namespace esphome {
namespace probreeze {

static const char *const TAG = "tuya.fan";

void ProBreezeFan::setup() {
  this->parent_->register_fan_state_listener([this](bool state) {
    this->state = state;
    this->publish_state();
  });
  this->parent_->register_fan_speed_listener([this](ProBreeze::FanSpeed speed) {
    if (speed == ProBreeze::FanSpeed::LOW) {
      this->speed = 1;
    } else if (speed == ProBreeze::FanSpeed::HIGH) {
      this->speed = 2;
    }
    this->publish_state();
  });
}

fan::FanTraits ProBreezeFan::get_traits() {
  return fan::FanTraits(false, true, false, 2);
}

void ProBreezeFan::control(const fan::FanCall &call) {
  if (call.get_state().has_value()) {
    bool state = *call.get_state();
    
    if (call.get_speed().has_value()) {
      int speed = *call.get_speed();
      if (speed == 1) {
        this->parent_->set_fan_speed(ProBreeze::FanSpeed::LOW);
      } else if (speed == 2) {
        this->parent_->set_fan_speed(ProBreeze::FanSpeed::HIGH);
      }
      this->parent_->set_fan_state(state);
    } else {
      this->parent_->set_fan_speed(ProBreeze::FanSpeed::HIGH);
      this->parent_->set_fan_state(state);
    }
  }
}

}  // namespace probreeze
}  // namespace esphome
