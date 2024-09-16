#pragma once
#include "esphome.h"

class Rtl433ToMqtt : public esphome::Component, public esphome::mqtt::CustomMQTTDevice {
 public:
  Rtl433ToMqtt(const std::string &name, sensor::Sensor *sensor_1, sensor::Sensor *sensor_2)
      : name_(name), sensor_1_(sensor_1), sensor_2_(sensor_2) {}

  void setup() override {
    // Initialization logic
  }

  void loop() override {
    // Main loop
  }

  void process_json(JsonObject &doc) {
    int received_id = doc["id"] | 0;
    float current_value = doc["current"] | 0.0;

    // Update sensor based on the ID
    if (received_id == 50985) {
      sensor_1_->publish_state(current_value);
    } else if (received_id == 50986) {
      sensor_2_->publish_state(current_value);
    }
  }

 private:
  std::string name_;
  sensor::Sensor *sensor_1_;
  sensor::Sensor *sensor_2_;
};

  
  
    
