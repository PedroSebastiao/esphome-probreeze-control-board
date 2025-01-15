#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

namespace esphome {
namespace probreeze {

class ProBreeze : public esphome::Component, public esphome::uart::UARTDevice {
 public:
//   ProBreeze() : esphome::uart::UARTDevice(0) {}

  void setup() override {
    ESP_LOGD("probreeze", "ProBreeze Component setup complete");
  }

  void loop() override {
    while (available()) {
      char c = read();
      ESP_LOGD("probreeze", "Received: %c", c);

      // Echo received character back
      write(c);
    }
  }
};

}  // namespace probreeze
}  // namespace esphome