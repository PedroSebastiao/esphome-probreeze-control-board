#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace probreeze {

class ProBreeze : public esphome::Component, public esphome::uart::UARTDevice {
 public:
  void setup() override;
  void loop() override;

 protected:
  uint32_t last_transmission_;
};

}  // namespace probreeze
}  // namespace esphome