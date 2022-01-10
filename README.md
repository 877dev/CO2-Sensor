# Made 10-01-2022 by 877dev

# Quick description:
SCD30 combined CO2/temp/humidity sensor with LCD 1602 display.

# Operation:
The device connects to WiFi and MQTT broker and checks sensor is ok, showing the status at each stage. A heartbeat symbol flashes top right every 10 seconds to show the MQTT connection is alive.

# Hardware:
Blue backlight I2C LCD1602(1602 Serial LCD Module Display) brand GeekPi
Adafruit SCD30 sensor
Wemos D1 mini v4.0.0 is used for I2C cable connection

# Connections:
SCD30 via 3V/GND/SCL/SDA (i2c address 0x61)
LCD1602 via 5V/GND/SCL/SDA (i2c address 0x27)

# Code on my github:
https://github.com/877dev/CO2-Sensor

# 3D printable case including editable files:
https://www.thingiverse.com/thing:5195146

# Further reading:
https://wiki.52pi.com/index.php?title=Z-0234                  //GeekPi product official website
https://arduinogetstarted.com/tutorials/arduino-lcd-i2c
https://www.makerguides.com/character-i2c-lcd-arduino-tutorial

