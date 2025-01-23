#include "probreeze.h"

#include <string>
#include <vector>

#define SEND_EVERY_MILLIS 1000
#define DEBOUNCE_TIME 3000

namespace esphome {
namespace probreeze {

static const char *const TAG = "probreeze";

void ProBreeze::setup() {
    this->set_power_state(false);
    this->set_compressor_state(false);
    this->set_fan_state(false);
    this->set_fan_speed(HIGH);
    ESP_LOGD("probreeze", "ProBreeze Component setup complete");
}

void ProBreeze::loop() {
    const uint32_t now = millis();

    if (now >= this->tank_full_last_state_change_ + DEBOUNCE_TIME) {
        this->set_tank_full_debounced(this->tank_full_);
    }

    if (this->tank_full_debounced_) {
        this->set_compressor_state(false);
    } else {
        this->set_compressor_state(this->power_state_);
    }

    if (now - this->last_transmission_ > SEND_EVERY_MILLIS) {
        ESP_LOGD(TAG, "Will send state update command");
        this->last_transmission_ = now;

        this->send_message_data({ 0x10 }, true);

        uint8_t output = 0x00;
        if (this->compressor_state_) {
            if (!this->fan_state_) {
                this->set_fan_speed(HIGH);
                this->set_fan_state(true);
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
        this->set_humidity(data[1]);
        this->set_temperature(data[2]);
        

        bool tank_full = data[3] && 0x01;
        this->set_tank_full(tank_full);

        this->has_valid_state_ = true;
        ESP_LOGD(TAG, "Temperature: %u, Humidity: %u, Tank Full: %s, Tank Full (Debounced): %s", this->temperature_, this->humidity_, this->tank_full_ ? "yes" : "no", this->tank_full_debounced_ ? "yes" : "no");

    } else if (message_type == 0x01) {
        // set outputs
        // if it was received then it was successful
        ESP_LOGD(TAG, "Successfuly set outputs");
    }
}

void ProBreeze::set_power_state(bool state) {
    this->power_state_ = state;

    for (auto &listener: this->power_state_listeners_) {
        listener(this->power_state_);
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


void ProBreeze::set_temperature(int8_t temperature) {
    this->temperature_ = temperature;

    for (auto &listener: this->temperature_listeners_) {
        listener(this->temperature_);
    }
}

void ProBreeze::set_humidity(int8_t humidity) {
    this->humidity_ = humidity;

    for (auto &listener: this->humidity_listeners_) {
        listener(this->humidity_);
    }
}

void ProBreeze::set_tank_full(bool tank_full) {
    bool changed = this->tank_full_ != tank_full;

    this->tank_full_ = tank_full;

    for (auto &listener: this->tank_full_listeners_) {
        listener(this->tank_full_);
    }

    if (changed) {
        this->set_tank_full_last_state_change(millis());
    }
}

void ProBreeze::set_tank_full_debounced(bool tank_full_debounced) {
    this->tank_full_debounced_ = tank_full_debounced;

    for (auto &listener: this->tank_full_debounced_listeners_) {
        listener(this->tank_full_debounced_);
    }
}

void ProBreeze::set_tank_full_last_state_change(uint32_t tank_full_last_state_change) {
    this->tank_full_last_state_change_ = tank_full_last_state_change;
}

void ProBreeze::set_compressor_state(bool compressor_state) {
    this->compressor_state_ = compressor_state;

    for (auto &listener: this->compressor_state_listeners_) {
        listener(this->compressor_state_);
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

void ProBreeze::register_tank_full_debounced_listener(const std::function<void(bool)> &listener) {
    this->tank_full_debounced_listeners_.push_back(listener);

    listener(this->tank_full_debounced_);
}

void ProBreeze::register_power_state_listener(const std::function<void(bool)> &listener) {
    this->power_state_listeners_.push_back(listener);

    listener(this->power_state_);
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