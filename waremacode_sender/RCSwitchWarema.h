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
            /// @brief sends a warema master code
            /// @param codeWord code word to send
            /// @param dataLength data length
            /// @param syncLength sync length
            /// @param numberOfTransmissions number of transmissions
            /// @param sendDelay send delay
            void sendMC(std::string const &codeWord, unsigned int const &dataLength, unsigned int const &syncLength, uint8_t const &numberOfTransmissions, unsigned int const &sendDelay);
            void enableTransmit(int const &transmitterPin);

        private:
            int m_transmitterPin{};
        };

    } // namespace waremacode_sender
} // namespace esphome