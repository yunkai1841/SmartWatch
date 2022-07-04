
# SmartWatch   [![Badge License]][License]

*A project for the **Arduino UNO**.*

A watch that do not need RTL (real time clock). 
Display will be turned on automaticaly when you lift your arm. 
Also, you can control it by remote. Adjust clock. 

<br>

## Dependencies

- **[Ultrasonic-HC-SR04]**

- **[IRRemote]**

<br>

## Material list

- Arduino UNO
- 7 segment 4 digit LED
- 74HC595
- HC-SR04
- 220Ω resister × 4
- IR receiver
- IR remote (unknown)

- Thermistor (Focusens MF52)
- 10kΩ resister

Thermistor and 10kΩ resister is option. 
Those are not needed if you do not need to measure temperature.

If you use other remote or thermistor, 
`Source\Codes.cpp` and `Source\Temperature.cpp`
should be replaced. 

<br>

## Wiring Diagram

![Schematic]


<!----------------------------------------------------------------------------->

[Ultrasonic-HC-SR04]: https://github.com/toi-store/Ultrasonic-HC-SR04
[IRRemote]: https://github.com/Arduino-IRremote/Arduino-IRremote

[Schematic]: Resources/Schematic.png
[License]: #


<!--------------------------------[ Badges ]----------------------------------->

[Badge License]: https://img.shields.io/badge/License-Apache_2.0-D22128?style=for-the-badge
