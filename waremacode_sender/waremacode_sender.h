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

            void set_pin(InternalGPIOPin *pin) { m_transmitterPin = pin; }
            void set_number_of_transmissions(uint8_t const& number_of_transmissions) { m_numberOfTransmissions = number_of_transmissions; }

        private:
            void on_command_send_waremacode(const std::string &payload);

            unsigned int m_dataLength{1780};
            unsigned int m_syncLength{5000};
            uint8_t m_numberOfTransmissions{3};  // Default to 3 transmissions
            unsigned int m_sendDelay{10000};

            InternalGPIOPin *m_transmitterPin{nullptr};

            std::string m_topic{};
            RCSwitchWarema m_mySwitch{};
        };

    } // namespace waremacode_sender
} // namespace esphome