#pragma once

#include "esphome/core/component.h"
#include "esphome/components/probreeze/probreeze.h"
#include "esphome/components/switch/switch.h"

namespace esphome {
namespace probreeze {

class ProBreezeSwitch : public switch_::Switch, public Component {
 public:
  void setup() override;

  void set_probreeze_parent(ProBreeze *parent) { this->parent_ = parent; }

 protected:
  void write_state(bool state) override;

  ProBreeze *parent_;
};

}  // namespace probreeze
}  // namespace esphome
