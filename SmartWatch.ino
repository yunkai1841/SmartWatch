#include <SR04.h>

typedef unsigned long ul; 

typedef struct {
  ul second;
  ul minute;
  ul hour;
} eztm;

// constant define
const bool debug = true;

const int disp_interval = 50;

const int digit[] = {2, 3, 4, 5}; // 7seg pin 0~4
const int latch=9;  //74HC595  pin 9 STCP
const int clock=10; //74HC595  pin 10 SHCP
const int data=8;   //74HC595  pin 8 DS
const int trig = 6;
const int echo = 7;

const unsigned char table[]=
{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,
0x77,0x7c,0x39,0x5e,0x79,0x71,0x00};

//set clock start time for microsecond
ul initialtime;

SR04 sr04 = SR04(echo,trig);

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

void setup() {
  pinMode(latch,OUTPUT);
  pinMode(clock,OUTPUT);
  pinMode(data,OUTPUT);
  for (int i = 0; i < 4; i++)
  {
    pinMode(digit[i], OUTPUT);
    digitalWrite(digit[i], HIGH);
  }
 Serial.begin(9600);
 initialtime = 46530000; //set clock start time for microsecond
}

void loop() {
  // fetch distance
  long distance = sr04.Distance();
  if(debug) {
    Serial.print(distance);
    Serial.println("cm");
  }

  // calc current time
  ul cpu, real;
  cpu = millis();
  real = cpu + initialtime;
  if(debug) Serial.println(real);

  // calc which number to display
  eztm t = get_tm(real);
  ul hm = t.hour*100 + t.minute;
  if(debug) Serial.println(hm);
  if(distance < 30) Display4(hm);
  
//  delay(1000);
//  Serial.println("time is");
//  Serial.println(real);
//  Serial.print(t.hour);
//  Serial.print(":");
//  Serial.print(t.minute);
//  Serial.print(":");
//  Serial.println(t.second);
//  delay(1000);
}
