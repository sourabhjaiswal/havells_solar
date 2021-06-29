#include "havells_solar.h"
#include "havells_solar_registers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace havells_solar {

static const char *const TAG = "havells_solar";

static const uint8_t MODBUS_CMD_READ_IN_REGISTERS = 0x04;
static const uint8_t MODBUS_REGISTER_COUNT = 80;  // 74 x 16-bit registers

void HAVELLSSolar::on_modbus_data(const std::vector<uint8_t> &data) {
  if (data.size() < MODBUS_REGISTER_COUNT * 2) {
    ESP_LOGW(TAG, "Invalid size for HAVELLSSolar!");
    return;
  }

  auto havells_solar_get_float = [&](size_t i) -> float {
    uint32_t temp = encode_uint32(data[i], data[i + 1], data[i + 2], data[i + 3]);
    float f;
    memcpy(&f, &temp, sizeof(f));
    return f;
  };

  for (uint8_t i = 0; i < 3; i++) {
    auto phase = this->phases_[i];
    if (!phase.setup)
      continue;

    float voltage = havells_solar_get_float(HAVELLS_PHASE_1_VOLTAGE * 2 + (i * 4));
    float current = havells_solar_get_float(HAVELLS_PHASE_1_CURRENT * 2 + (i * 4));
    float active_power = havells_solar_get_float(HAVELLS_PHASE_1_ACTIVE_POWER * 2 + (i * 4));
    float reactive_power = havells_solar_get_float(HAVELLS_PHASE_1_REACTIVE_POWER * 2 + (i * 4));

    ESP_LOGD(
        TAG,
        "HAVELLSSolar Phase %c: V=%.3f V, I=%.3f A, Active P=%.3f W, Reactive P=%.3f VAR, "
        "",
        i + 'A', voltage, current, active_power, reactive_power);
    if (phase.voltage_sensor_ != nullptr)
      phase.voltage_sensor_->publish_state(voltage);
    if (phase.current_sensor_ != nullptr)
      phase.current_sensor_->publish_state(current);
    if (phase.active_power_sensor_ != nullptr)
      phase.active_power_sensor_->publish_state(active_power);
    if (phase.reactive_power_sensor_ != nullptr)
      phase.reactive_power_sensor_->publish_state(reactive_power);
  }

  float frequency = havells_solar_get_float(HAVELLS_FREQUENCY * 2);
  float active_energy = havells_solar_get_float(HAVELLS_ACTIVE_ENERGY * 2);
  float reactive_energy = havells_solar_get_float(HAVELLS_REACTIVE_ENERGY * 2);

  ESP_LOGD(TAG, "HAVELLSSolar: F=%.3f Hz, A.E=%.3f Wh, R.E=%.3f VARh", frequency,
           active_energy, reactive_energy);

  if (this->frequency_sensor_ != nullptr)
    this->frequency_sensor_->publish_state(frequency);
  if (this->active_energy_sensor_ != nullptr)
    this->active_energy_sensor_->publish_state(active_energy);
  if (this->reactive_energy_sensor_ != nullptr)
    this->reactive_energy_sensor_->publish_state(reactive_energy);
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
    LOG_SENSOR("    ", "Active Power", phase.active_power_sensor_);
    LOG_SENSOR("    ", "Reactive Power", phase.reactive_power_sensor_);
  }
  LOG_SENSOR("  ", "Frequency", this->frequency_sensor_);
  LOG_SENSOR("  ", "Active Energy", this->active_energy_sensor_);
  LOG_SENSOR("  ", "Reactive Energy", this->reactive_energy_sensor_);
}

}  // namespace havells_solar
}  // namespace esphome
