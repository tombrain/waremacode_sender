#include "esphome/core/log.h"
#include "waremacode_sender.h"
#include "esphome/core/helpers.h" // Für ESPHome-Mutex

#include <ArduinoJson.h>
#include <list>

namespace esphome
{
    namespace waremacode_sender
    {
        static const char *TAG = "waremacode_sender.component";

        class RollingCommandQueue
        {
            struct CommandEntry
            {
                std::string command;
                int remaining_transmissions;
            };

        public:
            RollingCommandQueue(int transmissions)
                : transmissions_(transmissions)
            {
            }

            void enqueue(const std::string &command)
            {
                queue_.push_back({command, transmissions_});
            }

            std::string dequeue()
            {
                auto entry{queue_.front()};
                queue_.pop_front();

                entry.remaining_transmissions -= 1;

                if (entry.remaining_transmissions > 0)
                {
                    queue_.push_back(entry);
                }

                return entry.command;
            }

            bool is_empty() const
            {
                return queue_.empty();
            }

        private:
            std::list<CommandEntry> queue_;
            int transmissions_;
        };

        std::unique_ptr<RollingCommandQueue> command_queue{};
        Mutex queue_mutex{};

        void WaremacodeSenderComponent::setup()
        {
            m_topic = mqtt::global_mqtt_client->get_topic_prefix() + "/command/send_waremacode";

            command_queue = std::make_unique<RollingCommandQueue>(m_numberOfTransmissions);

            ESP_LOGD(TAG, "setup()");

            {
                subscribe(m_topic, &WaremacodeSenderComponent::on_command_send_waremacode);
                ESP_LOGI(TAG, "subscribe: %s", m_topic.c_str());
            }
        }

        void WaremacodeSenderComponent::dump_config()
        {
            ESP_LOGCONFIG(TAG, "Used GPIO-Pin: %d", m_transmitterPin->get_pin());
            ESP_LOGCONFIG(TAG, "MQTT-Topic: %s", m_topic.c_str());
            ESP_LOGCONFIG(TAG, "Number of Transmissions: %d", m_numberOfTransmissions);
        }

        void WaremacodeSenderComponent::on_command_send_waremacode(const std::string &command)
        {
            ESP_LOGD(TAG, "on_command_send_waremacode command: '%s'", command.c_str());

            if (command.length() == 36)
            {
                LockGuard lock{queue_mutex};
                command_queue->enqueue(command);
                ESP_LOGD(TAG, "Command added to queue.");
            }
        }

        void WaremacodeSenderComponent::loop()
        {
            LockGuard lock{queue_mutex};

            if (!command_queue->is_empty())
            {
                auto const command{command_queue->dequeue()};

                ESP_LOGD(TAG, "Processing command from queue: '%s'", command.c_str());

                m_mySwitch.enableTransmit(m_transmitterPin->get_pin());
                m_mySwitch.sendMC(command, m_dataLength, m_syncLength, 1, m_sendDelay);
                m_mySwitch.disableTransmit();
            }
        }
    } // namespace waremacode_sender
} // namespace esphome