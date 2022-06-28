#include <SR04.h>
#include <IRremote.h>

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
const int IRreceiver = 11; // Signal Pin of IR receiver to Arduino Digital Pin 11

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
int keta = 0;
int setuptmp = 0;

// set up ultra sonic sensor and IR sensor
SR04 sr04 = SR04(echo,trig);
IRrecv irrecv(IRreceiver);
decode_results results;

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
  Serial.begin(9600);
  initialtime = 46530000; //set clock start time for microsecond
}

void loop() {
  if(debug) Serial.println("LOOP START");
  // control by remote
  if (irrecv.decode(&results)) {
    int a = IR2num();
    Serial.println(a);
    if(a == OK) {
      edit = true;
      keta = 0;
      setuptmp = 0;
    }
    if (edit && 0 <= a && 9 >= a) {
      setuptmp = (setuptmp * 10) + a;
      if(keta == 3) {
        edit = false;
      }
      keta ++;
    }
    
    irrecv.resume(); // receive the next value
    if(debug) Serial.println(setuptmp);
  }

  if (!edit) {
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
  }
  
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
