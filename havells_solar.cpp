#include "havells_solar.h"
#include "havells_solar_registers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace havells_solar {

static const char *const TAG = "havells_solar";

static const uint8_t MODBUS_CMD_READ_IN_REGISTERS = 0x03;
static const uint8_t MODBUS_REGISTER_COUNT = 48;  // 48 x 16-bit registers

void HAVELLSSolar::on_modbus_data(const std::vector<uint8_t> &data) {
  if (data.size() < MODBUS_REGISTER_COUNT * 2) {
    ESP_LOGW(TAG, "Invalid size for HAVELLSSolar!");
    return;
  }

  auto havells_solar_get_float = [&](size_t i) -> float {
    uint16_t temp = encode_uint16(data[i], data[i + 1]);
    float f;
    memcpy(&f, &temp, sizeof(f));
    return f;
  };

//  for (uint8_t i = 0; i < 3; i++) {
//    auto phase = this->phases_[i];
//    if (!phase.setup)
//      continue;
//
//    float voltage = havells_solar_get_float(HAVELLS_PHASE_1_VOLTAGE * 2 + (i * 4));
//    float current = havells_solar_get_float(HAVELLS_PHASE_1_CURRENT * 2 + (i * 4));
//
//    ESP_LOGD(
//        TAG,
//        "HAVELLSSolar Phase %c: V=%.3f V, I=%.3f A ",
//        i + 'A', voltage, current);
//    if (phase.voltage_sensor_ != nullptr)
//      phase.voltage_sensor_->publish_state(voltage);
//    if (phase.current_sensor_ != nullptr)
//      phase.current_sensor_->publish_state(current);
//  }

  float frequency = havells_solar_get_float(HAVELLS_GRID_FREQUENCY);
//  float active_power = havells_solar_get_float(HAVELLS_SYSTEM_ACTIVE_POWER);
//  float reactive_power = havells_solar_get_float(HAVELLS_SYSTEM_REACTIVE_POWER);

  ESP_LOGD(TAG, "HAVELLSSolar: F=%.3f Hz, Active P=%.3f W, Reactive P=%.3f VAR", frequency, active_power, reactive_power);

  if (this->frequency_sensor_ != nullptr)
    this->frequency_sensor_->publish_state(frequency);
  if (this->active_power_sensor_ != nullptr)
    this->active_power_sensor_->publish_state(active_power);
  if (this->reactive_power_sensor_ != nullptr)
    this->reactive_power_sensor_->publish_state(reactive_power);
}

void HAVELLSSolar::update() { this->send(MODBUS_CMD_READ_IN_REGISTERS, 0, MODBUS_REGISTER_COUNT); }
void HAVELLSSolar::dump_config() {
  ESP_LOGCONFIG(TAG, "HAVELLS Solar:");
  ESP_LOGCONFIG(TAG, "  Address: 0x%02X", this->address_);
  for (uint8_t i = 0; i < 3; i++) {
    auto phase = this->phases_[i];
    if (!phase.setup)
      continue;
    ESP_LOGCONFIG(TAG, "  Phase %c", i + 'A');
    LOG_SENSOR("    ", "Voltage", phase.voltage_sensor_);
    LOG_SENSOR("    ", "Current", phase.current_sensor_);
  }
  LOG_SENSOR("  ", "Frequency", this->frequency_sensor_);
  LOG_SENSOR("    ", "Active Power", this->active_power_sensor_);
  LOG_SENSOR("    ", "Reactive Power", this->reactive_power_sensor_);
}

}  // namespace havells_solar
}  // namespace esphome
