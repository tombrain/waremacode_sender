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
			// m_transmitterPin = pin_->get_pin();

			m_topicBase = mqtt::global_mqtt_client->get_topic_prefix();

			ESP_LOGI(TAG, "setup()");

			{
				std::string const t{m_topicBase + "/command/waremablind"};
				subscribe(t, &WaremacodeSenderComponent::on_command_waremasend);
				ESP_LOGI(TAG, "subscribe: %s", t.c_str());
			}
			{
				std::string const t{m_topicBase + "/command/waremablind_json"};
				subscribe(t, &WaremacodeSenderComponent::on_command_waremasend_json);
				ESP_LOGI(TAG, "subscribe: %s", t.c_str());
			}
			{
				std::string const t{m_topicBase + "/command/raw433"};
				subscribe(t, &WaremacodeSenderComponent::on_command_raw433);
				ESP_LOGI(TAG, "subscribe: %s", t.c_str());
			}
		}

		void WaremacodeSenderComponent::dump_config()
		{
			ESP_LOGCONFIG(TAG, "WaremacodeSenderComponent");
			ESP_LOGCONFIG(TAG, "Verwendeter GPIO-Pin: %d", m_transmitterPin);
			ESP_LOGCONFIG(TAG, "MQTT-Topic: %s", m_topicBase.c_str());
		}

		void WaremacodeSenderComponent::on_command_waremasend(const std::string &payload)
		{
			ESP_LOGD(TAG, "on_command_waremasend payload: '%s'", payload.c_str());

			if (payload.length() == 36)
			{
				m_mySwitch.enableTransmit(m_transmitterPin);
				m_mySwitch.sendMC(payload, m_dataLength, m_syncLength, m_sendCommand, m_sendDelay);
				m_mySwitch.disableTransmit();
			}
		}

		void WaremacodeSenderComponent::on_command_waremasend_json(const std::string &payload)
		{
			ESP_LOGD(TAG, "on_command_waremasend_json payload: '%s'", payload.c_str());

			StaticJsonDocument<300> doc;

			// Deserialize the JSON document
			DeserializationError error = deserializeJson(doc, payload);

			if (error)
			{
				ESP_LOGD(TAG, "on_command_waremasend_json payload: deserializeJson() failed: '%s'", error.f_str());
			}
			else
			{
				ESP_LOGD(TAG, "on_command_waremasend_json: start");
				const char *codedevice_first{doc["codedevice_first"]};
				ESP_LOGD(TAG, "on_command_waremasend_json payload: codedevice_first '%s'", codedevice_first);
				const char *codedevice_second{doc["codedevice_second"]};
				ESP_LOGD(TAG, "on_command_waremasend_json payload: codedevice_second '%s'", codedevice_second);
				const char *code_action{doc["code_action"]};
				ESP_LOGD(TAG, "on_command_waremasend_json payload: code_action '%s'", code_action);

				unsigned int const data_length{doc["data_length"]};
				ESP_LOGD(TAG, "on_command_waremasend_json payload: data_length '%d'", data_length);
				if (data_length)
				{
					m_dataLength = data_length;
				}
				ESP_LOGD(TAG, "on_command_waremasend_json: m_dataLength '%d'", m_dataLength);
				unsigned int const sync_length{doc["sync_length"]};
				ESP_LOGD(TAG, "on_command_waremasend_json payload: sync_length '%d'", sync_length);
				if (sync_length)
				{
					m_syncLength = sync_length;
				}
				ESP_LOGD(TAG, "on_command_waremasend_json: m_syncLength '%d'", m_syncLength);
				uint8_t const num_commands{doc["num_commands"]};
				ESP_LOGD(TAG, "on_command_waremasend_json payload: num_commands '%d'", num_commands);
				if (num_commands)
				{
					m_sendCommand = num_commands;
				}
				ESP_LOGD(TAG, "on_command_waremasend_json: m_sendCommand '%d'", m_sendCommand);
				unsigned int const send_delay{doc["send_delay"]};
				ESP_LOGD(TAG, "on_command_waremasend_json payload: send_delay '%d'", send_delay);
				if (send_delay)
				{
					m_sendDelay = send_delay;
				}
				ESP_LOGD(TAG, "on_command_waremasend_json: m_sendDelay '%d'", m_sendDelay);

				std::string const command{std::string{codedevice_first} + std::string{codedevice_second} + std::string{code_action} + "S"};

				ESP_LOGD(TAG, "on_command_waremasend_json command: '%s'", command.c_str());
				if (command.length() == 36)
				{
					m_mySwitch.enableTransmit(m_transmitterPin);
					m_mySwitch.sendMC(command, m_dataLength, m_syncLength, m_sendCommand, m_sendDelay);
					m_mySwitch.disableTransmit();
				}
			}
		}

		void WaremacodeSenderComponent::on_command_raw433(const std::string &payload)
		{
			ESP_LOGD(TAG, "on_command_waremasend payload: '%s'", payload.c_str());

			m_mySwitch.enableTransmit(m_transmitterPin);
			m_mySwitch.send(payload.c_str());
			m_mySwitch.disableTransmit();
		}

		void WaremacodeSenderComponent::on_json_message(JsonObject &root)
		{
			if (!root.containsKey("key"))
				return;

			int value = root["key"];
			// do something with Json Object

			// publish JSON using lambda syntax
			// publish_json("the/other/json/topic", [=](JsonObject &root2) {
			//  root2["key"] = "Hello World";
			//});
		}

	} // namespace waremacode_sender
} // namespace esphome