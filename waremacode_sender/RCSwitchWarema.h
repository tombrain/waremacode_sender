#pragma once

#include "RCSwitch.h"

#include <string>

namespace esphome
{
    namespace waremacode_sender
    {
        class RCSwitchWarema : public RCSwitch
        {
            using Base = RCSwitch;

        public:
            void sendMC(std::string const &sCodeWord, unsigned int dataLength, unsigned int syncLength, uint8_t sendCommand, unsigned int sendDelay);
            void enableTransmit(int nTransmitterPin);

        private:
            int m_transmitterPin{};
        };

    } // namespace empty_component
} // namespace esphome