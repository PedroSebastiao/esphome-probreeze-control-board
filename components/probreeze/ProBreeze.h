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
  bool has_valid_state_;
  int8_t temperature_;
  uint8_t humidity_;
  bool tank_full_;
//   void sendMessage
};

}  // namespace probreeze
}  // namespace esphome