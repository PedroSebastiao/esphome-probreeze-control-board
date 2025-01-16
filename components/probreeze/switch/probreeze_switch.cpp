#include "esphome/core/log.h"
#include "probreeze_switch.h"

namespace esphome {
namespace probreeze {

static const char *const TAG = "probreeze.switch";

void ProBreezeSwitch::setup() {
  this->publish_state(false);
  // this->parent_->register_listener(this->switch_id_, [this](const TuyaDatapoint &datapoint) {
  //   ESP_LOGV(TAG, "MCU reported switch %u is: %s", this->switch_id_, ONOFF(datapoint.value_bool));
  //   this->publish_state(datapoint.value_bool);
  // });
}

void ProBreezeSwitch::write_state(bool state) {
  ESP_LOGV(TAG, "Setting switch %u: %s", this->switch_id_, ONOFF(state));
  this->parent_->set_compressor_state(state);
  this->publish_state(state);
}

}  // namespace probreeze
}  // namespace esphome
