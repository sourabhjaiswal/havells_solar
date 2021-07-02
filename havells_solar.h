#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/modbus/modbus.h"

namespace esphome {
namespace havells_solar {

class HAVELLSSolar : public PollingComponent, public modbus::ModbusDevice {
 public:
  void set_voltage_sensor(uint8_t phase, sensor::Sensor *voltage_sensor) {
    this->phases_[phase].setup = true;
    this->phases_[phase].voltage_sensor_ = voltage_sensor;
  }
  void set_current_sensor(uint8_t phase, sensor::Sensor *current_sensor) {
    this->phases_[phase].setup = true;
    this->phases_[phase].current_sensor_ = current_sensor;
  }
  
  void set_voltage_sensor_pv(uint8_t pv, sensor::Sensor *voltage_sensor) {
    this->pvs_[pv].setup = true;
    this->pvs_[pv].voltage_sensor_ = voltage_sensor;
  }
  void set_current_sensor_pv(uint8_t pv, sensor::Sensor *current_sensor) {
    this->pvs_[pv].setup = true;
    this->pvs_[pv].current_sensor_ = current_sensor;
  }
  
  void set_frequency_sensor(sensor::Sensor *frequency_sensor) { this->frequency_sensor_ = frequency_sensor; }
  void set_active_power_sensor(sensor::Sensor *active_power_sensor) { this->active_power_sensor_ = active_power_sensor; }
  void set_reactive_power_sensor(sensor::Sensor *reactive_power_sensor) { this->reactive_power_sensor_ = reactive_power_sensor; }
  void set_today_production_sensor(sensor::Sensor *today_production_sensor) { this->today_production_sensor_ = today_production_sensor; }

  void update() override;

  void on_modbus_data(const std::vector<uint8_t> &data) override;

  void dump_config() override;

 protected:
  struct HAVELLSPhase {
    bool setup{false};
    sensor::Sensor *voltage_sensor_{nullptr};
    sensor::Sensor *current_sensor_{nullptr};
  } phases_[3];
  struct HAVELLSPV {
    bool setup{false};
    sensor::Sensor *voltage_sensor_{nullptr};
    sensor::Sensor *current_sensor_{nullptr};
  } pvs_[2];
  sensor::Sensor *frequency_sensor_{nullptr};
  sensor::Sensor *active_power_sensor_{nullptr};
  sensor::Sensor *reactive_power_sensor_{nullptr};
  sensor::Sensor *today_production_sensor_{nullptr};
};

}  // namespace havells_solar
}  // namespace esphome
