# Soldered Ultrasonic Sensor With Qwiic ESPHome Component

| ![Ultrasonic Sensor With Qwiic](https://cms.soldered.com/products/333001/media/333001_featured-photo_cb09ce.jpg) |
| :-------------------------------------------------------------------------------------------------------------: |
|                           [Ultrasonic Sensor With Qwiic](https://www.solde.red/333001)                           |

Measures distances from 2 cm to 400 cm using the HC-SR04 ultrasonic sensor. An onboard ATtiny404 runs the
trigger/echo measurement and reports the result over I2C, so no extra GPIO pins are needed. The board is part of the
[Qwiic ecosystem](https://soldered.com/collections/qwiic-ecosystem).

External ESPHome component for the Soldered Ultrasonic Sensor with Qwiic. It is a port of the
[Soldered Ultrasonic Sensor easyC Arduino library](https://github.com/SolderedElectronics/Soldered-Ultrasonic-Sensor-easyC-Arduino-Library)
and publishes the distance as an ESPHome [sensor](https://esphome.io/components/sensor/) in meters, the same way the
built-in [`ultrasonic`](https://esphome.io/components/sensor/ultrasonic/) component does for a plain HC-SR04 wired to
trigger/echo GPIOs.

> For a plain [HC-SR04 module](https://www.solde.red/555041) without Qwiic (trigger/echo pins), use ESPHome's
> built-in `ultrasonic` component instead.

## Repository Contents

- **components/** - the ESPHome external component (Python config + C++ implementation)
- **examples/** - example YAML configs showing how to use the component

## Usage

Reference this repo directly from your own ESPHome YAML (no need to clone it locally):

```yaml
external_components:
  - source: github://SolderedElectronics/Soldered-Ultrasonic-Qwiic-ESPHome-Component
    components: [soldered_ultrasonic]

i2c:
  sda: GPIO21
  scl: GPIO22

sensor:
  - platform: soldered_ultrasonic
    name: "Distance"
    update_interval: 1s
```

On every update the component starts a measurement, waits 50 ms for the board to finish it (without blocking the
ESPHome main loop), then reads the echo time and converts it to a distance using a speed of sound of 343 m/s. When
nothing is in range the board reports no echo and the sensor publishes `NaN` (shown as "unknown" in Home Assistant).

See [`examples/basic.yaml`](examples/basic.yaml) for a full working example.

### Configuration variables

- **address** (*Optional*, int): I2C address of the board. Defaults to `0x30`; can be set to `0x30` - `0x37` with the
  board's three address-select pads (each one closed adds 1, 2 or 4).
- **update_interval** (*Optional*, [Time](https://esphome.io/guides/configuration-types#config-time)): how often to
  measure. Defaults to `60s`. Must be longer than 50 ms, the time one measurement takes.
- **i2c_id** (*Optional*, [ID](https://esphome.io/guides/configuration-types#config-id)): I2C bus to use, if there is
  more than one.
- All other options from [Sensor](https://esphome.io/components/sensor/#config-sensor) (`name`, `filters`,
  `unit_of_measurement`, ...). To get centimeters, use a `multiply: 100` filter and set `unit_of_measurement: cm`.

### Hardware design

You can find hardware design for this board in the
[_Ultrasonic sensor qwiic_](https://github.com/SolderedElectronics/Ultrasonic-sensor-qwiic-hardware-design) hardware
repository.

### Documentation

Access library documentation [here](https://docs.soldered.com/).

### About Soldered

<img src="https://raw.githubusercontent.com/SolderedElectronics/Soldered-Generic-Arduino-Library/dev/extras/Soldered-logo-color.png" alt="soldered-logo" width="500"/>

At Soldered, we design and manufacture a wide selection of electronic products to help you turn your ideas into acts and bring you one step closer to your final project. Our products are intented for makers and crafted in-house by our experienced team in Osijek, Croatia. We believe that sharing is a crucial element for improvement and innovation, and we work hard to stay connected with all our makers regardless of their skill or experience level. Therefore, all our products are open-source. Finally, we always have your back. If you face any problem concerning either your shopping experience or your electronics project, our team will help you deal with it, offering efficient customer service and cost-free technical support anytime. Some of those might be useful for you:

- [Web Store](https://www.soldered.com/shop)
- [Tutorials & Projects](https://soldered.com/learn)
- [Documentation](https://docs.soldered.com)

### Open-source license

Soldered invests vast amounts of time into hardware & software for these products, which are all open-source. Please support future development by buying one of our products.

Check license details in the LICENSE file. Long story short, use these open-source files for any purpose you want to, as long as you apply the same open-source licence to it and disclose the original source. No warranty - all designs in this repository are distributed in the hope that they will be useful, but without any warranty. They are provided "AS IS", therefore without warranty of any kind, either expressed or implied. The entire quality and performance of what you do with the contents of this repository are your responsibility. In no event, Soldered (TAVU) will be liable for your damages, losses, including any general, special, incidental or consequential damage arising out of the use or inability to use the contents of this repository.

## Have fun!

And thank you from your fellow makers at Soldered Electronics.
