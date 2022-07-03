#ifndef Header_Temperature
#define Header_Temperature


#include <Arduino.h>


namespace Temperature {


    /**
     * @brief Read the temperatur in K from the connected sensor
     */

    double inKelvin(uint8_t pin);


    /**
     * @brief Read the temperatur in ℃ from the connected sensor
     */

    double inCelcius(uint8_t pin);
}


#endif
