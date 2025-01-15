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

        // discard all unread bytes
        uint8_t byte;
        while (this->read_byte(&byte)) { }
        this->rx_message_.clear();
        
        std::vector<uint8_t> data = { 0x10 };
        Message msg(data);
        this->write_array(msg.rawMessage());
    }

    while (this->available()) {
        uint8_t byte;
        this->read_byte(&byte);
        this->handle_rx_byte_(byte);
    }
}

void ProBreeze::handle_rx_byte_(uint8_t byte) {
    this->rx_message_.push_back(byte);
    if (this->validate_rx_message_()) {
        this->rx_message_.clear();
    }
}

bool ProBreeze::validate_rx_message_() {
    uint8_t expectedSize = this->rx_message_.front();
    uint8_t size = this->rx_message_.size();

    if (size >= 2 && size == expectedSize) {
        std::vector<std::uint8_t> data(this->rx_message_.begin() + 1, this->rx_message_.end() - 1);
        Message expectedMessage(data);
        if (this->rx_message_ == expectedMessage.rawMessage()) {
            ESP_LOGD(TAG, "Got message with valid checksum");
            return true;
        } else {
            ESP_LOGD(TAG, "Got message with INVALID checksum");
            return false;
        }
    } else {
        return false;
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