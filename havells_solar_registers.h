#pragma once

namespace esphome {
namespace havells_solar {

/* Output Grid Message */
static const uint16_t HAVELLS_SYSTEM_ACTIVE_POWER = 0x000C;
static const uint16_t HAVELLS_SYSTEM_REACTIVE_POWER = 0x000D;
static const uint16_t HAVELLS_GRID_FREQUENCY = 0x000E;
static const uint16_t HAVELLS_PHASE_1_VOLTAGE = 0x000F;
static const uint16_t HAVELLS_PHASE_1_CURRENT = 0x0010;
static const uint16_t HAVELLS_PHASE_2_VOLTAGE = 0x0011;
static const uint16_t HAVELLS_PHASE_2_CURRENT = 0x0012;
static const uint16_t HAVELLS_PHASE_3_VOLTAGE = 0x0013;
static const uint16_t HAVELLS_PHASE_3_CURRENT = 0x0014;


/* PV Input Message */

static const uint16_t HAVELLS_PV_1_VOLTAGE = 0x0006;
static const uint16_t HAVELLS_PV_1_CURRENT = 0x0007;
static const uint16_t HAVELLS_PV_2_VOLTAGE = 0x0008;
static const uint16_t HAVELLS_PV_2_CURRENT = 0x0009;
static const uint16_t HAVELLS_PV_1_POWER = 0x000A;
static const uint16_t HAVELLS_PV_2_POWER = 0x000B;



}  // namespace havells_solar
}  // namespace esphome
