import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_PIN

waremacode_sender_ns = cg.esphome_ns.namespace("waremacode_sender")
WaremacodeSenderComponent = waremacode_sender_ns.class_("WaremacodeSenderComponent", cg.Component)

# Validierung für den GPIO-Pin (stellt sicher, dass es eine Zahl ist)
def validate_output_pin(value):
    # Stellen Sie sicher, dass es sich um eine gültige Zahl handelt (GPIO-Nummer)
    pin = cv.positive_int(value)  # Nur Zahlen sind erlaubt
    return pin
    
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(WaremacodeSenderComponent),
        cv.Required(CONF_PIN): validate_output_pin,  # GPIO-Pin als Pflichtfeld mit Output-Check
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    
    # Pin verarbeiten (Konvertierung der Pin-Nummer)
    pin = config[CONF_PIN]  # Die Pin-Nummer wird direkt verwendet
    cg.add(var.set_pin(pin))
    