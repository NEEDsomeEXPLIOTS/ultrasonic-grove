# ESPHome Grove Ultrasonic (single-wire SIG) component

Custom ESPHome sensor for the Grove Ultrasonic Ranger (single SIG pin),
e.g. with Seeed XIAO ESP32-S3.

## Usage

```yaml
external_components:
  - source: github://uspike/esphome-components
    components: [grove_ultrasonic]

esp32:
  board: esp32-s3-devkitc-1
  framework:
    type: arduino

sensor:
  - platform: grove_ultrasonic
    name: "Salt Tank Distance"
    pin: GPIO3          # whatever GPIO SIG is wired to
    update_interval: 1h
    filters:
      - multiply: 0.9525  # optional calibration

