#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

#include "esphome/components/mqtt/custom_mqtt_device.h"
// #include "esphome/components/binary_sensor/binary_sensor.h"

#include "RCSwitchWarema.h"

#include <string>

namespace esphome
{
    namespace waremacode_sender
    {

        class WaremacodeSenderComponent : public Component, public mqtt::CustomMQTTDevice
        {
        public:
            void setup() override;
            // void loop() override;
            void dump_config() override;

            void set_pin(InternalGPIOPin *pin) { pin_ = pin; }
            void set_pin(int const &pin) { m_transmitterPin = pin; }

        private:
            void on_command_waremasend(const std::string &payload);
            void on_command_waremasend_json(const std::string &payload);
            void on_command_raw433(const std::string &payload);
            void on_json_message(JsonObject &root);

            unsigned int m_dataLength{1780};
            unsigned int m_syncLength{5000};
            uint8_t m_sendCommand{3};
            unsigned int m_sendDelay{10000};

            uint8_t m_transmitterPin{};
            InternalGPIOPin *pin_;
            std::string m_topicBase{};
            RCSwitchWarema m_mySwitch{};
        };

    } // namespace empty_component
} // namespace esphome