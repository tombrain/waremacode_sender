#include "esphome/core/log.h"
#include "waremacode_sender.h"

#include <ArduinoJson.h>

namespace esphome
{
    namespace waremacode_sender
    {
        static const char *TAG = "waremacode_sender.component";

        void WaremacodeSenderComponent::setup()
        {
            m_topic = mqtt::global_mqtt_client->get_topic_prefix() + "/command/send_waremacode";

            ESP_LOGI(TAG, "setup()");

            {
                subscribe(m_topic, &WaremacodeSenderComponent::on_command_send_waremacode);
                ESP_LOGI(TAG, "subscribe: %s", m_topic.c_str());
            }
        }

        void WaremacodeSenderComponent::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Used GPIO-Pin: %d", m_transmitterPin);
            ESP_LOGCONFIG(TAG, "MQTT-Topic: %s", m_topic.c_str());
        }

        void WaremacodeSenderComponent::on_command_send_waremacode(const std::string &payload)
        {
            ESP_LOGD(TAG, "on_command_send_waremacode payload: '%s'", payload.c_str());

            if (payload.length() == 36)
            {
                m_mySwitch.enableTransmit(m_transmitterPin);
                m_mySwitch.sendMC(payload, m_dataLength, m_syncLength, m_sendCommand, m_sendDelay);
                m_mySwitch.disableTransmit();
            }
        }
    } // namespace waremacode_sender
} // namespace esphome