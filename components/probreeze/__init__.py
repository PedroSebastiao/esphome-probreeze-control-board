import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart

from esphome.const import (
    CONF_ID,
)

DEPENDENCIES = ["uart"]

# Declare the UART custom component namespace
probreeze_ns = cg.esphome_ns.namespace('probreeze')
ProBreeze = probreeze_ns.class_("ProBreeze", cg.Component, uart.UARTDevice)

# Configuration schema
CONF_PROBREEZE_ID = "probreeze_id"
CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(ProBreeze),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(uart.UART_DEVICE_SCHEMA)
)

# Code generation logic
async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)