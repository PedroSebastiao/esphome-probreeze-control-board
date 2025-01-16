from esphome.components import switch
import esphome.config_validation as cv
import esphome.codegen as cg
from .. import probreeze_ns, CONF_PROBREEZE_ID, ProBreeze

DEPENDENCIES = ["probreeze"]

ProBreezeSwitch = probreeze_ns.class_("ProBreezeSwitch", switch.Switch, cg.Component)

CONFIG_SCHEMA = (
    switch.switch_schema(ProBreezeSwitch)
    .extend(
        {
            cv.GenerateID(CONF_PROBREEZE_ID): cv.use_id(ProBreeze),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    var = await switch.new_switch(config)
    await cg.register_component(var, config)

    paren = await cg.get_variable(config[CONF_PROBREEZE_ID])
    cg.add(var.set_probreeze_parent(paren))
