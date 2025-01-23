from esphome.components import binary_sensor
import esphome.config_validation as cv
import esphome.codegen as cg
from .. import probreeze_ns, CONF_PROBREEZE_ID, ProBreeze

from esphome.const import (
    CONF_ID,
    DEVICE_CLASS_PROBLEM,
    DEVICE_CLASS_RUNNING,
)

DEPENDENCIES = ["probreeze"]

ProBreezeBinarySensor = probreeze_ns.class_("ProBreezeBinarySensor", cg.Component)

CONF_TANK_FULL = "tank_full"
CONF_COMPRESSOR = "compressor"
CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(ProBreezeBinarySensor),
            cv.GenerateID(CONF_PROBREEZE_ID): cv.use_id(ProBreeze),
            cv.Optional(CONF_TANK_FULL): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_PROBLEM,
            ),
            cv.Optional(CONF_COMPRESSOR): binary_sensor.binary_sensor_schema(
                device_class=DEVICE_CLASS_RUNNING,
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

    if CONF_TANK_FULL in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_TANK_FULL])
        cg.add(var.set_tank_full_binary_sensor(sens))

    if CONF_COMPRESSOR in config:
        sens = await binary_sensor.new_binary_sensor(config[CONF_COMPRESSOR])
        cg.add(var.set_compressor_binary_sensor(sens))
