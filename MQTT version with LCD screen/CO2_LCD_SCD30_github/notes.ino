/*
Project by 877dev, created 10th Jan 2022 (edited for github upload).

*********************************************************
**EDIT SECRET CREDENTIALS FILES WITH YOUR INFORMATION****
*********************************************************

Description:
------------
SCD30 CO2/temp/humidiy sensor with LCD 1602 display

Hardware:
---------
Blue backlight I2C LCD1602(1602 Serial LCD Module Display)
Adafruit SCD30 sensor
Wemos D1 mini v4.0.0 or similar

Connections:
------------
SCD30 via 3V/GND/SCL/SDA (i2c address 0x61)
LCD1602 via 5V/GND/SCL/SDA (i2c address 0x27)


Software:
---------
v001 CO2 monitor based on MQTT and heartbeat example, with GeekPi LCD matrix
v002 Heart symbol for heartbeat on LCD, error message on LCD before watchdog reset, increased watchdog reset to 30mins.
v003 Just some tidying up of code, added "readSCD30sensor" tab. Ignore CO2 values less than 400 (i.e. startup values).
     Added "lcd_on" and "lcd_off" mqtt callback commands. Add decimal place to temp LCD reading.
v004 Fixed spelling errors, tidied code a little.
v005 Just MQTT and WiFi config changes.


     
To do if want:
--------------
Physical button to dim display


LCD common commands
--------------------
lcd.backlight();    // turn on backlight.
lcd.noBacklight();  // turn off backlight


More info:
---------
https://wiki.52pi.com/index.php?title=Z-0234                  //product official website
https://arduinogetstarted.com/tutorials/arduino-lcd-i2c
https://www.makerguides.com/character-i2c-lcd-arduino-tutorial/


Misc project notes:
------------------
ESP will reset if hearbeat not received, either due to WiFi or MQTT not connecting.



*/
