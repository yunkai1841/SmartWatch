#include <Ultrasonic.h>
#include <IRremote.h>
// #include <SimpleDHT.h>


#define DEBUG


#include "Codes.hpp"
#include "Temperature.hpp"
#include "TimeStamp.hpp"
#include "Types.hpp"
#include "Debug.hpp"
#include "Pins.hpp"



// constant define
// const bool debug = false;



const unsigned char table [] = {
    0x3f , 0x06 , 0x5b , 0x4f , 0x66 , 0x6d ,
    0x7d , 0x07 , 0x7f , 0x6f , 0x77 , 0x7c ,
    0x39 , 0x5e , 0x79 , 0x71 , 0x00
};


// Set clock start time for microsecond

bool
    thermo = false ,
    edit = false ;

int
    edit_index = 0 ,
    setuptmp = 0 ;

ul initialtime;


// Set up ultra sonic sensor and IR sensor

Ultrasonic sr04 ( Pins::echo , Pins::trig );
IRrecv irrecv( Pins::IRreceiver );

decode_results results;

// SimpleDHT11 dht11;





void Display(unsigned char num){

    digitalWrite(Pins::latch,LOW);
    shiftOut(Pins::data,Pins::clock,MSBFIRST,table[num]);
    digitalWrite(Pins::latch,HIGH);
}

void Display4(ul x){

    for(int i = 0;i < 4;i++){

        digitalWrite(Pins::digit[i],LOW);

        Display(x % 10);

        delay(Pins::disp_interval);

        digitalWrite(Pins::digit[i],HIGH);

        x /= 10;
    }
}


void displayTemperature(){

    const double temperature =
        Temperature::inCelcius(Pins::thermometer);

    println(temperature);

    Display4((int) temperature);
}



void setup(){

    pinMode(Pins::latch,OUTPUT);
    pinMode(Pins::clock,OUTPUT);
    pinMode(Pins::data,OUTPUT);

    for(int i = 0;i < 4;i++){
        pinMode(Pins::digit[i],OUTPUT);
        digitalWrite(Pins::digit[i],HIGH);
    }

    irrecv.enableIRIn();

    // Set clock start time for microsecond

    initialtime = 43200000;

    #ifdef DEBUG
        Serial.begin(9600);
    #endif
}


void loop(){

    println("LOOP START");

    //  Control by remote

    if(irrecv.decode(& results)){

        int event = parseIRCode(results.value);

        delay(500);

        print("IR receive: ");
        println(event);

        switch(event){
        case Ok :
            edit = true;
            edit_index = 0;
            setuptmp = 0;
            break;
        case Up :
            thermo = true;;
            break;
        case Down :
            thermo = false;
            break;
        default:

            if(!edit)
                break;


            // Edit time mode

            if(0 <= event && 9 >= event){

                setuptmp = (setuptmp * 10) + event;

                if(edit_index == 3){

                    // edit state is end
                    // calculate and set initial time

                    int hourtmp = setuptmp / 100;
                    int minutetmp = setuptmp % 100;

                    ul tmp = (hourtmp * 60 + minutetmp) * 60 * 1000;

                    initialtime = tmp - millis();

                    println(initialtime);

                    edit = false;
                }

                edit_index++;
            }
        }


        // Receive the next value

        irrecv.resume();

        // Display setup tmp

        println(setuptmp);
    }


    // DH11 read temperature and humidity
    // dont work maybe out of memory
    /*   if (thermo) {
    byte temperature = 0;
    byte humidity = 0;
    byte DH11rawdata[40] = {0};
    if (dht11.read(12, &temperature, &humidity, DH11rawdata)) {
    Serial.println("Read DHT11 failed");
    return;
    }
    if (debug) {
    Serial.print("Sample RAW Bits: ");
    for (int i = 0; i < 40; i++) {
    Serial.print((int)DH11rawdata[i]);
    if (i > 0 && ((i + 1) % 4) == 0) {
    Serial.print(' ');
    }
    }
    Serial.println("");

    Serial.print("Sample OK: ");
    Serial.print((int)temperature); Serial.print(" *C, ");
    Serial.print((int)humidity); Serial.println(" %");
    }

    Display4((ul)temperature);
    delay(1000);
    Display4((ul)humidity);
    delay(1000);
    }
    */


    if(thermo){
        displayTemperature();
        return;
    }

    if(edit){
        Display4(setuptmp);
        return;
    }

    // Fetch distance

    long distance = sr04.Ranging(CM);

    print(distance);
    println("cm");

    // Calc current time

    ul
        cpu = millis() ,
        real = cpu + initialtime ;

    println(real);

    // Calc which number to display

    TimeStamp time(real);
    ul hm = time.hour * 100 + time.minute;

    println(hm);

    if(distance < 30)
        Display4(hm);

    println("time is");
    println(real);
    print(time.hour);
    print(":");
    print(time.minute);
    print(":");
    println(time.second);
}
