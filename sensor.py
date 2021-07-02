from esphome.components.atm90e32.sensor import CONF_PHASE_A, CONF_PHASE_B, CONF_PHASE_C
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, modbus
from esphome.const import (
    CONF_ACTIVE_POWER,
    CONF_CURRENT,
    CONF_FREQUENCY,
    CONF_ID,
    CONF_POWER_FACTOR,
    CONF_REACTIVE_POWER,
    CONF_VOLTAGE,
    CONF_ENERGY,
    DEVICE_CLASS_CURRENT,
    DEVICE_CLASS_EMPTY,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_POWER,
    DEVICE_CLASS_POWER_FACTOR,
    DEVICE_CLASS_VOLTAGE,
    ICON_CURRENT_AC,
    ICON_EMPTY,
    ICON_FLASH,
    STATE_CLASS_MEASUREMENT,
    STATE_CLASS_NONE,
    UNIT_AMPERE,
    UNIT_DEGREES,
    UNIT_EMPTY,
    UNIT_HERTZ,
    UNIT_VOLT,
    UNIT_VOLT_AMPS,
    UNIT_VOLT_AMPS_REACTIVE,
    UNIT_VOLT_AMPS_REACTIVE_HOURS,
    UNIT_WATT,
    UNIT_WATT_HOURS,
    UNIT_MINUTE,
)

CONF_ENERGY_PRODUCTION_DAY = "energy_production_day"
CONF_TOTAL_ENERGY_PRODUCTION = "total_energy_production"
CONF_TOTAL_GENERAION_TIME = "total_generation_time"
CONF_TODAY_GENERAION_TIME = "today_generation_time"
CONF_PV1 = "pv_1"
CONF_PV2 = "pv_2"
UNIT_KILOWATT_HOURS = "kWh"
UNIT_HOURS = "hrs"

AUTO_LOAD = ["modbus"]
CODEOWNERS = ["@sourabhjaiswal"]

havells_solar_ns = cg.esphome_ns.namespace("havells_solar")
HAVELLSSolar = havells_solar_ns.class_("HAVELLSSolar", cg.PollingComponent, modbus.ModbusDevice)

PHASE_SENSORS = {
    CONF_VOLTAGE: sensor.sensor_schema(UNIT_VOLT, ICON_EMPTY, 2, DEVICE_CLASS_VOLTAGE),
    CONF_CURRENT: sensor.sensor_schema(
        UNIT_AMPERE, ICON_EMPTY, 2, DEVICE_CLASS_CURRENT, STATE_CLASS_MEASUREMENT
    ),
}
PV_SENSORS = {
    CONF_VOLTAGE: sensor.sensor_schema(UNIT_VOLT, ICON_EMPTY, 2, DEVICE_CLASS_VOLTAGE),
    CONF_CURRENT: sensor.sensor_schema(
        UNIT_AMPERE, ICON_EMPTY, 2, DEVICE_CLASS_CURRENT, STATE_CLASS_MEASUREMENT
    ),
    CONF_ACTIVE_POWER: sensor.sensor_schema(
        UNIT_WATT, ICON_EMPTY, 0, DEVICE_CLASS_POWER, STATE_CLASS_MEASUREMENT
    ),
}

PHASE_SCHEMA = cv.Schema(
    {cv.Optional(sensor): schema for sensor, schema in PHASE_SENSORS.items()}
)
PV_SCHEMA = cv.Schema(
    {cv.Optional(sensor): schema for sensor, schema in PV_SENSORS.items()}
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(HAVELLSSolar),
            cv.Optional(CONF_PHASE_A): PHASE_SCHEMA,
            cv.Optional(CONF_PHASE_B): PHASE_SCHEMA,
            cv.Optional(CONF_PHASE_C): PHASE_SCHEMA,
            cv.Optional(CONF_PV1): PV_SCHEMA,
            cv.Optional(CONF_PV2): PV_SCHEMA,
            cv.Optional(CONF_FREQUENCY): sensor.sensor_schema(
                UNIT_HERTZ,
                ICON_CURRENT_AC,
                2,
                DEVICE_CLASS_EMPTY,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ACTIVE_POWER): sensor.sensor_schema(
                UNIT_WATT,
                ICON_EMPTY,
                0,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT
            ),
            cv.Optional(CONF_REACTIVE_POWER): sensor.sensor_schema(
                UNIT_VOLT_AMPS_REACTIVE,
                ICON_EMPTY,
                2,
                DEVICE_CLASS_POWER,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_ENERGY_PRODUCTION_DAY): sensor.sensor_schema(
                UNIT_KILOWATT_HOURS,
                ICON_EMPTY,
                2,
                DEVICE_CLASS_ENERGY,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TOTAL_ENERGY_PRODUCTION): sensor.sensor_schema(
                UNIT_KILOWATT_HOURS,
                ICON_EMPTY,
                0,
                DEVICE_CLASS_ENERGY,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TOTAL_GENERAION_TIME): sensor.sensor_schema(
                UNIT_HOURS,
                ICON_EMPTY,
                0,
                DEVICE_CLASS_EMPTY,
                STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_TODAY_GENERAION_TIME): sensor.sensor_schema(
                UNIT_MINUTE,
                ICON_EMPTY,
                0,
                DEVICE_CLASS_EMPTY,
                STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.polling_component_schema("10s"))
    .extend(modbus.modbus_device_schema(0x01))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await modbus.register_modbus_device(var, config)

    if CONF_FREQUENCY in config:
        sens = await sensor.new_sensor(config[CONF_FREQUENCY])
        cg.add(var.set_frequency_sensor(sens))
        
    if CONF_ACTIVE_POWER in config:
        sens = await sensor.new_sensor(config[CONF_ACTIVE_POWER])
        cg.add(var.set_active_power_sensor(sens))
        
    if CONF_REACTIVE_POWER in config:
        sens = await sensor.new_sensor(config[CONF_REACTIVE_POWER])
        cg.add(var.set_reactive_power_sensor(sens))

    if CONF_ENERGY_PRODUCTION_DAY in config:
        sens = await sensor.new_sensor(config[CONF_ENERGY_PRODUCTION_DAY])
        cg.add(var.set_today_production_sensor(sens))

    if CONF_TOTAL_ENERGY_PRODUCTION in config:
        sens = await sensor.new_sensor(config[CONF_TOTAL_ENERGY_PRODUCTION])
        cg.add(var.set_total_energy_production_sensor(sens))

    if CONF_TOTAL_GENERAION_TIME in config:
        sens = await sensor.new_sensor(config[CONF_TOTAL_GENERAION_TIME])
        cg.add(var.set_total_generation_time_sensor(sens))

    if CONF_TODAY_GENERAION_TIME in config:
        sens = await sensor.new_sensor(config[CONF_TODAY_GENERAION_TIME])
        cg.add(var.set_today_generation_time_sensor(sens))

    for i, phase in enumerate([CONF_PHASE_A, CONF_PHASE_B, CONF_PHASE_C]):
        if phase not in config:
            continue

        phase_config = config[phase]
        for sensor_type in PHASE_SENSORS:
            if sensor_type in phase_config:
                sens = await sensor.new_sensor(phase_config[sensor_type])
                cg.add(getattr(var, f"set_{sensor_type}_sensor")(i, sens))
                
    for i, pv in enumerate([CONF_PV1, CONF_PV2]):
        if pv not in config:
            continue

        pv_config = config[pv]
        for sensor_type in pv_config:
            if sensor_type in pv_config:
                sens = await sensor.new_sensor(pv_config[sensor_type])
                cg.add(getattr(var, f"set_{sensor_type}_sensor_pv")(i, sens))
