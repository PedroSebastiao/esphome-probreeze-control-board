#include "ProBreeze.h"

namespace esphome {
namespace probreeze {


  void ProBreeze::setup() {
    ESP_LOGD("probreeze", "ProBreeze Component setup complete");
  }

  void ProBreeze::loop() {
    while (available()) {
      char c = read();
      ESP_LOGD("probreeze", "Received: %c", c);

      // Echo received character back
      write(c);
    }
  }


}  // namespace probreeze
}  // namespace esphome