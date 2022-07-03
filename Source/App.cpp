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
#include "Display.hpp"



// constant define
// const bool debug = false;


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



    /*

    // DH11 read temperature and humidity
    // dont work maybe out of memory

    if(thermo){

        byte temperature = 0;
        byte humidity = 0;
        byte DH11rawdata[40] = {0};

        if(dht11.read(12,& temperature,& humidity,DH11rawdata)){
            println("Read DHT11 failed");
            return;
        }

        if(debug){

            print("Sample RAW Bits: ");

            for(int i = 0;i < 40; i++){
                print((int) DH11rawdata[i]);

                if(i > 0 && ((i + 1) % 4) == 0)
                    print(' ');
            }

            println("");

            print("Sample OK: ");
            print((int) temperature);
            print(" *C, ");
            print((int)humidity);
            println(" %");
        }

        Display::digits((ul) temperature);

        delay(1000);

        Display::digits((ul) humidity);

        delay(1000);
    }

    */


    if(thermo){
        Display::temperature();
        return;
    }

    if(edit){
        Display::digits(setuptmp);
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
        Display::digits(hm);

    println("time is");
    println(real);
    print(time.hour);
    print(":");
    print(time.minute);
    print(":");
    println(time.second);
}
