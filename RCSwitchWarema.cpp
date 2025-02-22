
#include "RCSwitchWarema.h"
#include "esphome/core/log.h"

namespace esphome
{
	namespace waremacode_sender
	{
		void RCSwitchWarema::sendMC(std::string const &sCodeWord, unsigned int dataLength, unsigned int syncLength, uint8_t sendCommand, unsigned int sendDelay)
		{
			ESP_LOGD("RCSwitchWarema", "sendMC: %s, %i, %i, %i, %i",
					 sCodeWord.c_str(),
					 dataLength,
					 syncLength,
					 sendCommand,
					 sendDelay);

			auto const halfDataLength{dataLength / 2};
			for (uint8_t nRepeat{0}; nRepeat < sendCommand; ++nRepeat)
			{
				digitalWrite(m_transmitterPin, LOW);

				for (auto const &c : sCodeWord)
				{
					switch (c)
					{
					case 's':
						digitalWrite(m_transmitterPin, LOW);
						delayMicroseconds(syncLength);
						break;

					case 'S':
						digitalWrite(m_transmitterPin, HIGH);
						delayMicroseconds(syncLength);
						break;

					case '0':
						digitalWrite(m_transmitterPin, HIGH);
						delayMicroseconds(halfDataLength);
						digitalWrite(m_transmitterPin, LOW);
						delayMicroseconds(halfDataLength);
						break;

					case '1':
						digitalWrite(m_transmitterPin, LOW);
						delayMicroseconds(halfDataLength);
						digitalWrite(m_transmitterPin, HIGH);
						delayMicroseconds(halfDataLength);
						break;
					}
				}
				digitalWrite(this->m_transmitterPin, LOW);
				if (nRepeat != sendCommand - 1)
				{
					delayMicroseconds(sendDelay);
				}
			}
			ESP_LOGD("RCSwitchWarema", "sendMC: ready");
		}

		void RCSwitchWarema::enableTransmit(int nTransmitterPin)
		{
			ESP_LOGI("RCSwitchWarema", "enableTransmit: %d", nTransmitterPin);
			m_transmitterPin = nTransmitterPin;

			Base::enableTransmit(nTransmitterPin);
		}

	} // namespace empty_component
} // namespace esphome