# Rtl433ToMqtt ESPHome Custom Component

This is a custom ESPHome component that processes RTL433 RF signals and publishes them via MQTT.

## Installation

1. Clone this repository or reference it directly in your `ESPHome` YAML configuration.

## Usage

In your `ESPHome` configuration YAML, add the following:

```yaml
external_components:
  - source: github://yourusername/my-rtl433-mqtt-component

sensor:
  - platform: custom
    lambda: |-
      auto rtl433 = new Rtl433ToMqtt("rtl_source");
      App.register_component(rtl433);
      return { rtl433->sensor_1_, rtl433->sensor_2_ };
    sensors:
      - name: "RTL433 Sensor 1"
      - name: "RTL433 Sensor 2"
