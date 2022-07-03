
#include <Arduino.h>
#include "Temperature.hpp"


//  Where do these come from?

const double
    a = 0.0000000876741 ,
    c = 0.001129148 ,
    b = 0.000234125 ;


double Temperature::inKelvin(uint8_t pin){

    double reading = analogRead(pin);

    //  To Percentage

    reading = 1024.0 / reading;

    reading -= 1;

    reading *= 10000;

    reading = log(reading);

    double kelvin = reading * reading * a;

    kelvin += b;

    kelvin *= reading;

    kelvin += c;

    return 1 / kelvin;
}


double Temperature::inCelcius(uint8_t pin){
    return inKelvin(pin) - 273.15;
}
