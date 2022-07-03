#include <SR04.h>
#include <IRremote.h>
// #include <SimpleDHT.h>


#define DEBUG


#ifdef DEBUG

    #define println(args)   \
        Serial.println(args);

    #define print(args)   \
        Serial.print(args);

#else

    #define println(args)
    #define print(args)

#endif


using ul = unsigned long;

typedef struct {

    ul second , minute , hour;

    eztm(ul millis){

        second = millis / 1000 ;
        minute = second / 60 ;
        hour = minute / 60 ;

        second %= 60;
        minute %= 60;
        hour %= 24;
    }

} eztm;

// constant define
// const bool debug = false;

const int digit[] = {2, 3, 4, 5}; // 7seg pin 0~4

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

#define UP 20
#define DOWN 21
#define LEFT 22
#define RIGHT 23
#define OK 24
#define STAR 25
#define SHARPP 26
#define REPT -2
#define OTHER -1

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

SR04 sr04 = SR04(echo,trig);
IRrecv irrecv(IRreceiver);
decode_results results;

// SimpleDHT11 dht11;





void Display(unsigned char num){

    digitalWrite(latch,LOW);
    shiftOut(data,clock,MSBFIRST,table[num]);
    digitalWrite(latch,HIGH);
}

void Display4(ul x){

    for(int i = 0;i < 4;i++){

        digitalWrite(digit[i],LOW);

        Display(x % 10);

        delay(disp_interval);

        digitalWrite(digit[i],HIGH);

        x /= 10;
    }
}


// Translate raw IR code to int

int IR2num(){
    switch(results.value){

    default : return OTHER ;

    case 0xFF4AB5 : return 0 ;
    case 0xFF6897 : return 1 ;
    case 0xFF9867 : return 2 ;
    case 0xFFB04F : return 3 ;
    case 0xFF30CF : return 4 ;
    case 0xFF18E7 : return 5 ;
    case 0xFF7A85 : return 6 ;
    case 0xFF10EF : return 7 ;
    case 0xFF38C7 : return 8 ;
    case 0xFF5AA5 : return 9 ;

    case 0xFFC23D : return RIGHT ;
    case 0xFF22DD : return LEFT ;
    case 0xFFA857 : return DOWN ;
    case 0xFF629D : return UP ;
    case 0xFF02FD : return OK ;

    case 0xFFFFFFFF : return REPT ;
    case 0xFF52AD : return SHARPP ;
    case 0xFF42BD : return STAR ;
    }

    delay(500);
}


void setup(){

    pinMode(latch,OUTPUT);
    pinMode(clock,OUTPUT);
    pinMode(data,OUTPUT);

    for(int i = 0;i < 4;i++){
        pinMode(digit[i],OUTPUT);
        digitalWrite(digit[i],HIGH);
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

        int a = IR2num();

        print("IR receive: ");
        println(a);

        switch(a){
        case OK :
            edit = true;
            edit_index = 0;
            setuptmp = 0;
            break;
        case UP :
            thermo = true;;
            break;
        case DOWN :
            thermo = false;
            break;
        }

        // Edit time mode

        if(edit && 0 <= a && 9 >= a){

            setuptmp = (setuptmp * 10) + a;

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

        // receive the next value

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

    // Thermometer program using Thermistor

    if(thermo){

        int tempReading = analogRead(thermometer);

        // This is OK

        double kelvin = log(10000.0 * ((1024.0 / tempReading - 1)));

        kelvin = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );

        // Convert Kelvin to Celcius

        float celcius = kelvin - 273.15;

        println(celcius);

        Display4((int) celcius);

        return;
    }

    if(edit){
        Display4(setuptmp);
        return;
    }

    // Fetch distance

    long distance = sr04.Distance();

    print(distance);
    println("cm");

    // Calc current time

    ul
        cpu = millis() ,
        real = cpu + initialtime ;

    println(real);

    // Calc which number to display

    eztm t = eztm(real);
    ul hm = t.hour * 100 + t.minute;

    println(hm);

    if(distance < 30)
        Display4(hm);

    println("time is");
    println(real);
    print(t.hour);
    print(":");
    print(t.minute);
    print(":");
    println(t.second);
}
