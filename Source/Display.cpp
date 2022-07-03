
#include "Temperature.hpp"
#include "Display.hpp"
#include "Debug.hpp"
#include "Pins.hpp"


const unsigned char table [] = {
    0x3f , 0x06 , 0x5b , 0x4f , 0x66 , 0x6d ,
    0x7d , 0x07 , 0x7f , 0x6f , 0x77 , 0x7c ,
    0x39 , 0x5e , 0x79 , 0x71 , 0x00
};


using namespace Pins;


void Display::Display(unsigned char num){

    digitalWrite(latch,LOW);
    shiftOut(data,clock,MSBFIRST,table[num]);
    digitalWrite(latch,HIGH);
}


void Display::digits(ul number){

    for(int i = 0;i < 4;i++){

        digitalWrite(digit[i],LOW);

        Display(number % 10);

        delay(disp_interval);

        digitalWrite(digit[i],HIGH);

        number /= 10;
    }
}


void Display::temperature(){

    const double temperature =
        Temperature::inCelcius(thermometer);

    println(temperature);

    digits((int) temperature);
}

