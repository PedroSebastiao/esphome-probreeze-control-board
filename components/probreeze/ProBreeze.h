#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"
#include "esphome/core/log.h"

#include "Message.h"

namespace esphome {
namespace probreeze {

class ProBreeze : public esphome::Component, public esphome::uart::UARTDevice {
    public:
        void setup() override;
        void loop() override;

        void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
        void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }

        // void register_temperature_listener(const uint8_t &func);
        // void register_humidity_listener(const uint8_t &func);

        enum FanSpeed {
            HIGH,
            LOW,
        };

        void set_compressor_state(bool state);
        void set_fan_state(bool state);
        void set_fan_speed(enum FanSpeed speed);

    protected:
        uint32_t last_transmission_;
        bool has_valid_state_;
        int8_t temperature_;
        uint8_t humidity_;
        bool tank_full_;

        std::vector<uint8_t> rx_message_;
        
        void send_message_data(std::vector<uint8_t> data, bool discard_unread_bytes);
        void handle_rx_byte_(uint8_t byte);
        bool validate_rx_message_();
        void process_message_(Message message);
        
        sensor::Sensor *temperature_sensor_{nullptr};
        sensor::Sensor *humidity_sensor_{nullptr};

        bool compressor_state_{false};
        bool fan_state_{false};
        enum FanSpeed fan_speed_{HIGH};

        // std::vector<std::function<void(uint8_t)>> temperature_listeners_;
        // std::vector<std::function<void(uint8_t)>> humidity_listeners_;
};

}  // namespace probreeze
}  // namespace esphome