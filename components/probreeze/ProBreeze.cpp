#include "ProBreeze.h"

#include "Message.h"

#include <string>
#include <vector>

#define SEND_EVERY_MILIS 1000

namespace esphome {
namespace probreeze {

static const char *const TAG = "probreeze";

void ProBreeze::setup() {
    this->has_valid_state_ = false;
    this->temperature_ = 0;
    this->humidity_ = 0;
    this->tank_full_ = true;
    ESP_LOGD("probreeze", "ProBreeze Component setup complete");
}

void ProBreeze::loop() {
    const uint32_t now = millis();
    if (now - this->last_transmission_ > SEND_EVERY_MILIS) {
        // ESP_LOGD(TAG, "'%s': Will send state update command", this->get_name().c_str());
        ESP_LOGD(TAG, "Will send state update command");
        // this->write_command_(this->state);
        this->last_transmission_ = now;

        std::vector<uint8_t> data = { 0x10 };
        Message msg(data);
        Message::RawMessage rawMessage = msg.rawMessage();
        this->write_array(rawMessage.bytes, rawMessage.totalSize);
    }
}

// void ProBreeze::sendMessage_(Message message) {

// }

// void UARTSwitch::write_command_(bool state) {
//     if (state && !this->data_on_.empty()) {
//         ESP_LOGD(TAG, "'%s': Sending on data...", this->get_name().c_str());
//         this->write_array(this->data_on_.data(), this->data_on_.size());
//     }
//     if (!state && !this->data_off_.empty()) {
//         ESP_LOGD(TAG, "'%s': Sending off data...", this->get_name().c_str());
//         this->write_array(this->data_off_.data(), this->data_off_.size());
//     }
// }

// void ProBreeze::loop() {
//     while (available()) {
//         char c = read();
//         ESP_LOGD("probreeze", "Received: %c", c);

//         // Echo received character back
//         write(c);
//     }
// }


}  // namespace probreeze
}  // namespace esphome