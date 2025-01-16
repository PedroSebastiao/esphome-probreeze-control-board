#pragma once

#include "esphome/core/component.h"
#include "esphome/components/probreeze/probreeze.h"
#include "esphome/components/fan/fan.h"

namespace esphome {
namespace probreeze {

class ProBreezeFan : public Component, public fan::Fan {
 public:
  ProBreezeFan(ProBreeze *parent) : parent_(parent) {}
  void setup() override;

  fan::FanTraits get_traits() override;

 protected:
  void control(const fan::FanCall &call) override;

  ProBreeze *parent_;
};

}  // namespace probreeze
}  // namespace esphome
