#pragma once

#include "esphome.h"

namespace probreeze {

class ProBreeze : public Component, public UARTDevice {
 public:
  ProBreeze() : ProBreeze(0) {}

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