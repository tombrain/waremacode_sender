import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PIN
from esphome import pins

# Import the rc-switch library
cg.add_library("rc-switch", "2.6.4")

waremacode_sender_ns = cg.esphome_ns.namespace("waremacode_sender")
WaremacodeSenderComponent = waremacode_sender_ns.class_("WaremacodeSenderComponent", cg.Component)

CONF_NUMBER_OF_TRANSMISSIONS = "number_of_transmissions"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(WaremacodeSenderComponent),
        cv.Required(CONF_PIN): pins.gpio_output_pin_schema,  # GPIO-Pin as a required field with GPIO check
        cv.Optional(CONF_NUMBER_OF_TRANSMISSIONS, default=3): cv.int_range(min=3, max=10),  # Number of transmissions, default 3, min 3, max 10
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    # Process pin (convert pin number)
    pin = await cg.gpio_pin_expression(config[CONF_PIN])  # The pin number is processed as a GPIO pin
    cg.add(var.set_pin(pin))
    
    # Set the number of transmissions
    cg.add(var.set_number_of_transmissions(config[CONF_NUMBER_OF_TRANSMISSIONS]))
