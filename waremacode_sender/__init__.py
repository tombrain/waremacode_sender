import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PIN
from esphome import pins

waremacode_sender_ns = cg.esphome_ns.namespace("waremacode_sender")
WaremacodeSenderComponent = waremacode_sender_ns.class_("WaremacodeSenderComponent", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(WaremacodeSenderComponent),
        cv.Required(CONF_PIN): pins.gpio_output_pin_schema,  # GPIO-Pin als Pflichtfeld mit GPIO-Check
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    # Pin verarbeiten (Konvertierung der Pin-Nummer)
    pin = await cg.gpio_pin_expression(config[CONF_PIN])  # Die Pin-Nummer wird als GPIO-Pin verarbeitet
    cg.add(var.set_pin(pin))
