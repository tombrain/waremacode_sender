#pragma once

#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

#include "esphome/components/mqtt/custom_mqtt_device.h"

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

            void set_pin(int const &pin) { m_transmitterPin = pin; }

        private:
            void on_command_send_waremacode(const std::string &payload);

            unsigned int m_dataLength{1780};
            unsigned int m_syncLength{5000};
            uint8_t m_sendCommand{3};
            unsigned int m_sendDelay{10000};

            uint8_t m_transmitterPin{};

            std::string m_topic{};
            RCSwitchWarema m_mySwitch{};
        };

    } // namespace empty_component
} // namespace esphome