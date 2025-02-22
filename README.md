How ot use: https://github.com/jesserockz/esphome-external-component-examples

Structure of files:

```
esphome
├── components
|   ├── waremacode_sender
|   │   ├── __init__.py
|   │   ├── RCSwitchWarema.cpp
|   │   ├── RCSwitchWarema.h
|   │   ├── waremacode_sender.cpp
|   │   ├── waremacode_sender.h
│  ...
```

```yaml
# example configuration:

esphome:
  name: waremacontrol
  libraries:
    - "rc-switch"

external_components:
  - source: components
  
esp8266:
  board: nodemcuv2

mqtt:
  broker: {{mroker}}
  username: mqtt-user
  password: !secret mqtt_password

# Enable logging
logger:

# Enable Home Assistant API
api:
  encryption:
    key: ...

ota:
  password: ...

wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password

  # Enable fallback hotspot (captive portal) in case wifi connection fails
  ap:
    ssid: "Waremacontrol Fallback Hotspot"
    password: ...

captive_portal:

cover:
  - platform: time_based
    name: "Rollo1"
    has_built_in_endstop: true

    open_action:
      - mqtt.publish:
          topic: waremacontrol/command/send_waremacode
          payload: "S01110100111111S010011100S000010001S"
    open_duration: 24sec

    close_action:
      - mqtt.publish:
          topic: waremacontrol/command/send_waremacode
          payload: "S01110100111110S010011100S000010001S"
    close_duration: 24sec

    stop_action:
      - mqtt.publish:
          topic: waremacontrol/command/send_waremacode
          payload: "S00110100111001S010011100S000010001S"

  - platform: time_based
    name: "Rollo2"
    has_built_in_endstop: true

    open_action:
      - mqtt.publish:
          topic: waremacontrol/command/send_waremacode
          payload: "S01110100110111S010111001S000111101S"
    open_duration: 24sec

    close_action:
      - mqtt.publish:
          topic: waremacontrol/command/send_waremacode
          payload: "S01110100110110S010111001S000111101S"
    close_duration: 24sec

    stop_action:
      - mqtt.publish:
          topic: waremacontrol/command/send_waremacode
          payload: "S00110100110001S010111001S000111101S"

waremacode_sender:
  pin: GPIO05
```
