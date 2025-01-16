from esphome.components import fan
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_OUTPUT_ID
from .. import probreeze_ns, CONF_PROBREEZE_ID, ProBreeze

DEPENDENCIES = ["probreeze"]

ProBreezeFan = probreeze_ns.class_("ProBreezeFan", cg.Component, fan.Fan)

CONFIG_SCHEMA = cv.All(
    fan.FAN_SCHEMA.extend(
        {
            cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(ProBreezeFan),
            cv.GenerateID(CONF_PROBREEZE_ID): cv.use_id(ProBreeze),
        }
    ).extend(cv.COMPONENT_SCHEMA),
)


async def to_code(config):
    parent = await cg.get_variable(config[CONF_PROBREEZE_ID])

    var = cg.new_Pvariable(config[CONF_OUTPUT_ID], parent)
    await cg.register_component(var, config)
    await fan.register_fan(var, config)
