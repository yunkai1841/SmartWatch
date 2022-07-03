#ifndef Header_Pins
#define Header_Pins


namespace Pins {


    // 7seg pin 0~4

    const int digit [] = { 2 , 3 , 4 , 5 };

    const int
        disp_interval = 100 ,
        thermometer = 0 ,       //  Analog pin for thermomistor
        IRreceiver = 11 ,       //  Signal Pin of IR receiver to Arduino Digital Pin 11
        clock = 10 ,            //  74HC595 pin 10 SHCP
        latch = 9 ,             //  74HC595 pin 9 STCP
        data = 8 ,              //  74HC595 pin 8 DS
        echo = 7 ,
        trig = 6 ;

    // int pinDHT11 = 12;
}


#endif
