#pragma once

#include "esphome/core/component.h"
#include "esphome/components/probreeze/probreeze.h"
#include "esphome/components/sensor/sensor.h"

namespace esphome {
namespace probreeze {

class ProBreezeSensor : public Component {
 public:
  void setup() override;

  void set_probreeze_parent(ProBreeze *parent) { this->parent_ = parent; }

  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }

 protected:
  ProBreeze *parent_;

  sensor::Sensor *temperature_sensor_{nullptr};
  sensor::Sensor *humidity_sensor_{nullptr};
};

}  // namespace probreeze
}  // namespace esphome
