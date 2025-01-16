#pragma once

#include "esphome/core/component.h"
#include "esphome/components/probreeze/probreeze.h"
#include "esphome/components/binary_sensor/binary_sensor.h"

namespace esphome {
namespace probreeze {

class ProBreezeBinarySensor : public Component {
 public:
  void setup() override;

  void set_probreeze_parent(ProBreeze *parent) { this->parent_ = parent; }

  void set_tank_full_binary_sensor(binary_sensor::BinarySensor *tank_full_binary_sensor) { tank_full_binary_sensor_ = tank_full_binary_sensor; }

 protected:
  ProBreeze *parent_;

  binary_sensor::BinarySensor *tank_full_binary_sensor_{nullptr};
};

}  // namespace probreeze
}  // namespace esphome
