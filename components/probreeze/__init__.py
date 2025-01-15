import esphome.codegen as cg
import esphome.config_validation as cv

DEPENDENCIES = ["uart"]

# Declare the UART custom component namespace
probreeze_ns = cg.esphome_ns.namespace('probreeze')
ProBreeze = probreeze_ns.class_("ProBreeze", cg.Component, uart.UARTDevice)

# Configuration schema
CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(ProBreeze),
}).extend(cv.COMPONENT_SCHEMA)

# Code generation logic
def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()])
    yield cg.register_component(var, config)