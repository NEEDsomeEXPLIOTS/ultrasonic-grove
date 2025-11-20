import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.const import (
    CONF_ID,
    CONF_PIN,
    CONF_UPDATE_INTERVAL,
    UNIT_CENTIMETER,
    ICON_ARROW_EXPAND_VERTICAL,
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT,
)

from esphome.components import sensor
import esphome.pins as pins

grove_ultrasonic_ns = cg.esphome_ns.namespace("grove_ultrasonic")
GroveUltrasonicSensor = grove_ultrasonic_ns.class_(
    "GroveUltrasonicSensor",
    cg.PollingComponent,
    sensor.Sensor,
)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_CENTIMETER,
    icon=ICON_ARROW_EXPAND_VERTICAL,
    accuracy_decimals=0,
    device_class=DEVICE_CLASS_DISTANCE,
    state_class=STATE_CLASS_MEASUREMENT,
).extend(
    {
        cv.GenerateID(): cv.declare_id(GroveUltrasonicSensor),
        cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
        cv.Optional(CONF_UPDATE_INTERVAL, default="1s"): cv.update_interval,
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)

    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))

