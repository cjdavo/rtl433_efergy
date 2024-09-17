#include "esphome.h"
#undef yield
#undef millis
#undef micros
#undef delay
#undef delayMicroseconds

#include "rtl_433_ESP.h"
#include <map>
#include <string>

class Rtl433ToMqtt : public Component, public mqtt::CustomMQTTDevice {
 public:
  Rtl433ToMqtt(const char* source) : source_(source) {
    instance_ = this;
  }

  static Rtl433ToMqtt* get(const custom_component::CustomComponentConstructor& c) {
    return static_cast<Rtl433ToMqtt*>(c.get_component(0));
  }

  void setup() override {
    rf_.initReceiver(RF_MODULE_RECEIVER_GPIO, RF_MODULE_FREQUENCY);
    rf_.setCallback(&Rtl433ToMqtt::process_dispatch, buffer_, sizeof(buffer_));
    rf_.enableReceiver();
  }

  void loop() override {
    rf_.loop();
  }

  void stop() {
    rf_.disableReceiver();
  }

 private:
  const char* source_;
  char buffer_[512];
  rtl_433_ESP rf_;
  static Rtl433ToMqtt* instance_;

  sensor::Sensor* sensor_1_;
  sensor::Sensor* sensor_2_;

  int received_id_1_;
  int received_id_2_;

  static void process_dispatch(char* msg) {
    if (instance_ != nullptr) instance_->process(msg);
  }

  void process(char* msg) {
    ESP_LOGD("custom", "Received msg: %s", msg);
    parse_json(msg, [this](JsonObject doc) {
      process_json(doc);
      return true;
    });
  }

  void process_json(JsonObject doc) {
    const char* received_model = doc["model"];
    int received_id_value = id(received_id);
    int received_id_value2 = id(received_id2);

    int received_id_from_signal = doc["id"] | 0;
    float current_value = doc["current"] | 0.0;

    if (received_id_value == received_id_from_signal) {
      sensor_1->publish_state(current_value);
    } else if (received_id_value2 == received_id_from_signal) {
      sensor_2->publish_state(current_value);
    }

    ESP_LOGI("custom", "Updated sensor for model: %s with ID: %d", received_model, received_id_from_signal);
  }
};

Rtl433ToMqtt* Rtl433ToMqtt::instance_ = nullptr;
