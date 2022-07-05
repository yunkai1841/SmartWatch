
# SmartWatch   [![Badge License]][License]

*A project for the **Arduino UNO**.*

<br>

This watch doesn't require a **R**eal **T**ime **C**lock, <br>
it's display automatically turns on when you <br>
lift your arm and you can remote control it.

<br>

## Dependencies

- **[Ultrasonic-HC-SR04]**

- **[IRRemote]**

<br>
<br>

## Parts List

-   **7 Segment 4 Digit LED**
-   **4 x 220Ω Resistor**
-   **Arduino UNO**
-   **IR Receiver**
-   **IR Remote**

    *Unknown*

-   **74HC595**
-   **HC-SR04**


<br>

### Optional

*Only required for measuring temperature.*

-   **`10㏀` Resistor**
-   **Thermistor**

    *Focusens MF52*

<br>

### Note

If you use a different remote or thermistor, you will have to <br>
replace  `Source\Codes.cpp`  &  `Source\Temperature.cpp` .

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
