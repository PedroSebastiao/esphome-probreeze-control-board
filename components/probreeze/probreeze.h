#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

#include "message.h"

namespace esphome {
namespace probreeze {

class ProBreeze : public esphome::Component, public esphome::uart::UARTDevice {
    public:
        enum FanSpeed {
            HIGH,
            LOW,
        };

        void setup() override;
        void loop() override;

        void register_temperature_listener(const std::function<void(uint8_t)> &listener);
        void register_humidity_listener(const std::function<void(uint8_t)> &listener);
        void register_tank_full_listener(const std::function<void(bool)> &listener);
        void register_tank_full_debounced_listener(const std::function<void(bool)> &listener);

        void register_power_state_listener(const std::function<void(bool)> &listener);
        void register_compressor_state_listener(const std::function<void(bool)> &listener);
        void register_fan_state_listener(const std::function<void(bool)> &listener);
        void register_fan_speed_listener(const std::function<void(enum FanSpeed)> &listener);

        void set_power_state(bool state);
        void set_fan_state(bool state);
        void set_fan_speed(enum FanSpeed speed);

    protected:
        uint32_t last_transmission_;
        bool has_valid_state_{false};
        int8_t temperature_{-128};
        uint8_t humidity_{255};
        bool tank_full_{true};
        uint32_t tank_full_last_state_change_{0xFFFFFFFF};
        bool tank_full_debounced_{true};

        std::vector<uint8_t> rx_message_;
        
        void send_message_data(std::vector<uint8_t> data, bool discard_unread_bytes);
        void handle_rx_byte_(uint8_t byte);
        bool validate_rx_message_();
        void process_message_(Message message);

        void set_temperature(int8_t temperature);
        void set_humidity(int8_t humidity);
        void set_tank_full(bool tank_full);
        void set_tank_full_debounced(bool tank_full_debounced);
        void set_tank_full_last_state_change(uint32_t tank_full_last_state_change);
        void set_compressor_state(bool state);
        void set_compressor_state_last_state_change(uint32_t compressor_state_last_state_change);

        bool power_state_{false};
        bool compressor_state_{false};
        uint32_t compressor_state_last_state_change_{0xFFFFFFFF};
        bool fan_state_{false};
        enum FanSpeed fan_speed_{HIGH};

        std::vector<std::function<void(uint8_t)>> temperature_listeners_;
        std::vector<std::function<void(uint8_t)>> humidity_listeners_;
        std::vector<std::function<void(bool)>> tank_full_listeners_;
        std::vector<std::function<void(bool)>> tank_full_debounced_listeners_;

        std::vector<std::function<void(bool)>> power_state_listeners_;
        std::vector<std::function<void(bool)>> compressor_state_listeners_;
        std::vector<std::function<void(bool)>> fan_state_listeners_;
        std::vector<std::function<void(enum FanSpeed)>> fan_speed_listeners_;
};

}  // namespace probreeze
}  // namespace esphome