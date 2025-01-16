from esphome.components import sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID
from .. import probreeze_ns, CONF_PROBREEZE_ID, ProBreeze

from esphome.const import (
    CONF_HUMIDITY,
    CONF_ID,
    CONF_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_PERCENT,
    DEVICE_CLASS_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
)

DEPENDENCIES = ["probreeze"]

ProBreezeSensor = probreeze_ns.class_("ProBreezeSensor", cg.Component)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(ProBreezeSensor),
            cv.GenerateID(CONF_PROBREEZE_ID): cv.use_id(ProBreeze),
            cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_TEMPERATURE,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
            cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(
                unit_of_measurement=UNIT_PERCENT,
                accuracy_decimals=0,
                device_class=DEVICE_CLASS_HUMIDITY,
                state_class=STATE_CLASS_MEASUREMENT,
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    paren = await cg.get_variable(config[CONF_PROBREEZE_ID])
    cg.add(var.set_probreeze_parent(paren))

    if CONF_TEMPERATURE in config:
        sens = await sensor.new_sensor(config[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
    if CONF_HUMIDITY in config:
        sens = await sensor.new_sensor(config[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))
