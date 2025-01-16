#include "probreeze.h"

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
        ESP_LOGD(TAG, "Will send state update command");
        this->last_transmission_ = now;

        this->send_message_data({ 0x10 }, true);

        uint8_t output = 0x00;
        if (this->compressor_state_) {
            if (!this->fan_state_) {
                this->set_fan_speed(HIGH);
                this->set_fan_state(true);
                // this->fan_speed_ = HIGH;
                // this->fan_state_ = true;
            }
            output |= 0b00000100;
        }
        if (this->fan_state_) {
            if (this->fan_speed_ == LOW) {
                output |= 0b00001010;
            } else {
                output |= 0b00001001;
            }
        }
        this->send_message_data({ 0x01, output}, false);
    }

    while (this->available()) {
        uint8_t byte;
        this->read_byte(&byte);
        this->handle_rx_byte_(byte);
    }
}

void ProBreeze::send_message_data(std::vector<uint8_t> data, bool discard_unread_bytes) {
    if (discard_unread_bytes) {
        // discard all unread bytes
        uint8_t byte;
        while (this->available()) { 
            this->read_byte(&byte);
        }
        this->rx_message_.clear();
    }
    
    Message message(data);
    this->write_array(message.rawMessage());
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
        Message message(data);
        if (this->rx_message_ == message.rawMessage()) {
            ESP_LOGD(TAG, "Got message with valid checksum");
            this->process_message_(message);
            return true;
        } else {
            ESP_LOGD(TAG, "Got message with INVALID checksum");
            return false;
        }
    } else {
        return false;
    }
}

void ProBreeze::process_message_(Message message) {
    auto data = message.data;
    uint8_t message_type = data[0];

    if (message_type == 0x10) {
        // get inputs
        this->humidity_ = data[1];
        this->temperature_ = data[2];
        this->tank_full_ = data[3] && 0x01;
        this->has_valid_state_ = true;
        ESP_LOGD(TAG, "Temperature: %u, Humidity: %u, Tank Full: %s", this->temperature_, this->humidity_, this->tank_full_ ? "yes" : "no");

        for (auto &listener: this->temperature_listeners_) {
            listener(this->temperature_);
        }
        for (auto &listener: this->humidity_listeners_) {
            listener(this->humidity_);
        }
        for (auto &listener: this->tank_full_listeners_) {
            listener(this->tank_full_);
        }

    } else if (message_type == 0x01) {
        // set outputs
        // if it was received then it was successful
        ESP_LOGD(TAG, "Successfuly set outputs");
    }
}

void ProBreeze::set_compressor_state(bool state) {
    this->compressor_state_ = state;

    for (auto &listener: this->compressor_state_listeners_) {
        listener(this->compressor_state_);
    }
}

void ProBreeze::set_fan_state(bool state) {
    this->fan_state_ = state;

    for (auto &listener: this->fan_state_listeners_) {
        listener(this->fan_state_);
    }
}

void ProBreeze::set_fan_speed(enum FanSpeed speed) {
    this->fan_speed_ = speed;

    for (auto &listener: this->fan_speed_listeners_) {
        listener(this->fan_speed_);
    }
}

void ProBreeze::register_temperature_listener(const std::function<void(uint8_t)> &listener) {
    this->temperature_listeners_.push_back(listener);

    listener(this->temperature_);
}

void ProBreeze::register_humidity_listener(const std::function<void(uint8_t)> &listener) {
    this->humidity_listeners_.push_back(listener);

    listener(this->humidity_);
}

void ProBreeze::register_tank_full_listener(const std::function<void(bool)> &listener) {
    this->tank_full_listeners_.push_back(listener);

    listener(this->tank_full_);
}

void ProBreeze::register_compressor_state_listener(const std::function<void(bool)> &listener) {
    this->compressor_state_listeners_.push_back(listener);

    listener(this->compressor_state_);
}

void ProBreeze::register_fan_state_listener(const std::function<void(bool)> &listener) {
    this->fan_state_listeners_.push_back(listener);

    listener(this->fan_state_);
}

void ProBreeze::register_fan_speed_listener(const std::function<void(enum FanSpeed)> &listener) {
    this->fan_speed_listeners_.push_back(listener);

    listener(this->fan_speed_);
}


}  // namespace probreeze
}  // namespace esphome