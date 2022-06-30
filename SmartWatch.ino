#include <SR04.h>
#include <IRremote.h>
// #include <SimpleDHT.h>

typedef unsigned long ul; 

typedef struct {
  ul second;
  ul minute;
  ul hour;
} eztm;

// constant define
// const bool debug = false;

const int disp_interval = 100;

const int digit[] = {2, 3, 4, 5}; // 7seg pin 0~4
const int latch=9;  //74HC595  pin 9 STCP
const int clock=10; //74HC595  pin 10 SHCP
const int data=8;   //74HC595  pin 8 DS
const int trig = 6;
const int echo = 7;
const int IRreceiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11
// int pinDHT11 = 12;
const int thermometer = 0; // Analog pin for thermomistor

#define UP 20
#define DOWN 21
#define LEFT 22
#define RIGHT 23
#define OK 24
#define STAR 25
#define SHARPP 26
#define REPT -2
#define OTHER -1

const unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

//set clock start time for microsecond
ul initialtime;
bool edit = false;
bool thermo = false;
int edit_index = 0;
int setuptmp = 0;

// set up ultra sonic sensor and IR sensor
SR04 sr04 = SR04(echo,trig);
IRrecv irrecv(IRreceiver);
decode_results results;
// SimpleDHT11 dht11;

//custom function
eztm get_tm(ul ms) {
  eztm t;
  t.second = ms / 1000;
  t.minute = t.second / 60;
  t.hour = t.minute / 60;
  t.second %= 60;
  t.minute %= 60;
  t.hour %= 24;
  return t;
}

void Display(unsigned char num)
{

  digitalWrite(latch,LOW);
  shiftOut(data,clock,MSBFIRST,table[num]);
  digitalWrite(latch,HIGH);
  
}
void Display4(ul x) {
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(digit[i], LOW);
    Display(x % 10);
    delay(disp_interval);
    digitalWrite(digit[i], HIGH);
    x /= 10;
  }
}

// translate raw IR code to int
int IR2num() {
  switch (results.value) {
    case 0xFF4AB5: return 0; break;
    case 0xFF6897: return 1; break;
    case 0xFF9867: return 2; break;
    case 0xFFB04F: return 3; break;
    case 0xFF30CF: return 4; break;
    case 0xFF18E7: return 5; break;
    case 0xFF7A85: return 6; break;
    case 0xFF10EF: return 7; break;
    case 0xFF38C7: return 8; break;
    case 0xFF5AA5: return 9; break;

    case 0xFF629D: return UP; break;
    case 0xFFA857: return DOWN; break;
    case 0xFF22DD: return LEFT; break;
    case 0xFFC23D: return RIGHT; break;
    case 0xFF02FD: return OK; break;

    case 0xFF42BD: return STAR;   break;
    case 0xFF52AD: return SHARPP; break;
    case 0xFFFFFFFF: return REPT; break;  
    default: return OTHER; break;
  }
  delay(500);
}

void setup() {
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);
  for (int i = 0; i < 4; i++)
  {
    pinMode(digit[i], OUTPUT);
    digitalWrite(digit[i], HIGH);
  }
  irrecv.enableIRIn();
  // Serial.begin(9600);
  initialtime = 43200000; //set clock start time for microsecond
}

void loop() {
  // if(debug) Serial.println("LOOP START");
  // control by remote
  if (irrecv.decode(&results)) {
    int a = IR2num();
      // Serial.print("IR receive: ");
      // Serial.println(a);
    if(a == OK) {
      edit = true;
      edit_index = 0;
      setuptmp = 0;
    } else if (a == UP) {
      thermo = true;
    } else if (a == DOWN) {
      thermo = false;
    }
    
    // edit time mode
    if (edit && 0 <= a && 9 >= a) {
      setuptmp = (setuptmp * 10) + a;
      if(edit_index == 3) {
        // edit state is end
        // calculate and set initial time
        int hourtmp = setuptmp / 100;
        int minutetmp = setuptmp % 100;
        ul tmp = (hourtmp * 60 + minutetmp);
        tmp *= 60;
        tmp *= 1000;
        initialtime = tmp - millis();
        // if(debug) Serial.println(initialtime);
        edit = false;
      }
      edit_index++;
    }
    
    irrecv.resume(); // receive the next value
    // display setup tmp
    // if(debug) Serial.println(setuptmp);
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
  // thermometer program using Thermistor
  if (thermo) {
    int tempReading = analogRead(thermometer);
    // This is OK
    double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
    tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
    float tempC = tempK - 273.15;            // Convert Kelvin to Celcius
    // Serial.println(tempC);
    Display4((int) tempC);
  } else if (edit) {
    Display4(setuptmp);
  } else {
    // fetch distance
    long distance = sr04.Distance();
    // if(debug) {
      // Serial.print(distance);
      // Serial.println("cm");
    // }

    // calc current time
    ul cpu, real;
    cpu = millis();
    real = cpu + initialtime;
    // if(debug) Serial.println(real);

    // calc which number to display
    eztm t = get_tm(real);
    ul hm = t.hour*100 + t.minute;
    // if(debug) Serial.println(hm);
    if(distance < 30) Display4(hm);

    // if(debug) {
    //   Serial.println("time is");
    //   Serial.println(real);
    //   Serial.print(t.hour);
    //   Serial.print(":");
    //   Serial.print(t.minute);
    //   Serial.print(":");
    //   Serial.println(t.second);
    // }
  }
}
