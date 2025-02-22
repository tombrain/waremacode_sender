#include "RCSwitchWarema.h"
#include "esphome/core/log.h"

namespace esphome
{
    namespace waremacode_sender
    {
        void RCSwitchWarema::sendMC(std::string const &codeWord, unsigned int const& dataLength, unsigned int const& syncLength, uint8_t const& numberOfTransmissions, unsigned int const& sendDelay)
        {
            ESP_LOGD("RCSwitchWarema", "sendMC: %s, %i, %i, %i, %i",
                     codeWord.c_str(),
                     dataLength,
                     syncLength,
                     numberOfTransmissions,
                     sendDelay);

            auto const halfDataLength{dataLength / 2};
            for (uint8_t repeat{0}; repeat < numberOfTransmissions; ++repeat)
            {
                digitalWrite(m_transmitterPin, LOW);

                for (auto const &c : codeWord)
                {
                    if (c == 's' || c == 'S')
                    {
                        digitalWrite(m_transmitterPin, (c == 's') ? LOW : HIGH);
                        delayMicroseconds(syncLength);
                    }
                    else if (c == '0' || c == '1')
                    {
                        bool isZero = (c == '0');
                        digitalWrite(m_transmitterPin, isZero ? HIGH : LOW);
                        delayMicroseconds(halfDataLength);
                        digitalWrite(m_transmitterPin, isZero ? LOW : HIGH);
                        delayMicroseconds(halfDataLength);
                    }
                }
                digitalWrite(m_transmitterPin, LOW);
                if (repeat != numberOfTransmissions - 1)
                {
                    delayMicroseconds(sendDelay);
                }
            }
            ESP_LOGD("RCSwitchWarema", "sendMC: ready");
        }

        void RCSwitchWarema::enableTransmit(int const& transmitterPin)
        {
            ESP_LOGI("RCSwitchWarema", "enableTransmit: %d", transmitterPin);
            m_transmitterPin = transmitterPin;

            Base::enableTransmit(transmitterPin);
        }

    } // namespace waremacode_sender
} // namespace esphome