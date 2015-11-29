#include <Time.h>
#include <TimeAlarms.h>
#include "Wire.h"
#include "ds3231.h"
#include "rtc_ds3231.h"
//#include "pitches.h"
#include <DS3232RTC.h>  
#include <EEPROM.h>
#include <IRremote.h>
//#include <LiquidCrystal.h>
//#include "moduleAlarm.h"


int pos = 0;
int IR_RECV_PIN_2 = 2;

const int RELAY_PIN_10=10;
const int PIN_13=13;
const int PIN_FEEDER_7=7;
const int PIN_GfeedLIGHT=9;
int BUZZ_PIN_8 = 8;
int feedingLIGHT=0;
int feedLIGHT=0;
int feeder;
int address = 0;
byte value;
int actled=3;
int actfeed=1;
int addr=0;
long angka=0;
long angka2=0;
long angka3=0;
long angka4=0;
long angka5=0;
int led_on= 0;
boolean act_led= true;
// set light task 1
int jam_on=7;
int menit_on=10;
int detik_on=0;
int jam_off=11;
int menit_off=1;
int detik_off=0;
// set light task 2
int jam2_on=19;
int menit2_on=9;
int detik2_on=0;
int jam2_off=23;
int menit2_off=4;
int detik2_off=0;

IRrecv irx(IR_RECV_PIN_2);
decode_results results;


void setup(){
//  LiquidCrystal lcd(12, 11, 6, 5, 4, 3);
//  lcd.begin(16, 2);
//  // Print a message to the LCD.
//  lcd.print("hello, world!");
  pinMode(RELAY_PIN_10, OUTPUT); //define the pin
  pinMode(PIN_FEEDER_7, OUTPUT);
  pinMode(PIN_GfeedLIGHT, OUTPUT);
reload();}



void  loop(){ 
  IRrecvHandle();
  digitalClockDisplay();  
  feedingLight();  
  //feedingSignalLight();
  //delay 1 detik
// Alarm.delay(100);
}
void reload(){
  Serial.begin(9600);
  angka=0;
  angka2=0;
  angka4=0;
  angka5=0;
  feeder=0;
  
  feedingLIGHT=0;
  feedLIGHT=0;
//  digitalWrite (led,LOW);
 // pinMode(PIN_13, OUTPUT); //define the pin

  irx.enableIRIn(); // Start the receiver
  //pinMode(REL_PIN, OUTPUT);
   // Serial.begin(9600);
    setSyncProvider(RTC.get);   // the function to get the time from the RTC
    if(timeStatus() != timeSet) 
        Serial.println("Unable to sync with the RTC");
    else
        Serial.println("RTC has set the system time");  
  //Alarm.timerRepeat(180, fishFeeding);
 //}
 setupAlarm();
 powerfailureRecovery();
  //setTime(20,25,10,11,9,15);
  //Alarm.alarmRepeat(dowSaturday,8,10,30,WeeklyAlarm);  // alarm tiap hr Sabtu 8:30:30

//Alarm.alarmRepeat(20,53,0, lightON);  // alarm tiap 8:1
  //Alarm.alarmRepeat(20,59,0, lightOFF);  // alarm tiap 8:1
 pinMode(BUZZ_PIN_8, OUTPUT);
      digitalWrite(BUZZ_PIN_8, 1);
      delay(500);
     digitalWrite(BUZZ_PIN_8, 0);
}

 void doubleBEEP(){
      digitalWrite(BUZZ_PIN_8, 1);
      delay(50);
     digitalWrite(BUZZ_PIN_8, 0);
      delay(50);
     digitalWrite(BUZZ_PIN_8, 1);
      delay(50);
     digitalWrite(BUZZ_PIN_8, 0);
 }
void setupAlarm(){
   int val = actled;
  actled = EEPROM.read(addr);
  //actled =3;
  //led_on = EEPROM.read(addr);
  //Alarm.alarmRepeat(23,46,5, lightON);  // alarm tiap 8:1
  if(actled==1){
  Alarm.alarmRepeat(jam_on, menit_on, detik_on, lightON);  // alarm tiap 8:1
  Alarm.alarmRepeat(jam_off,menit_off,detik_off, lightOFF);  // alarm tiap 8:1
  Serial.print("alarm has been set on: ");
  Serial.print(jam_on + " " + menit_on);
  Serial.println();
  }
  else if (actled==2){
  Alarm.alarmRepeat(jam2_on, menit2_on, detik2_on, lightON);  // alarm tiap 8:1
  Alarm.alarmRepeat(jam2_off,menit2_off,detik2_off, lightOFF);  // alarm tiap 8:1
  }
  else if (actled==3){
    Alarm.alarmRepeat(jam_on, menit_on, detik_on, lightON);  // alarm tiap 8:1
  Alarm.alarmRepeat(jam_off,menit_off,detik_off, lightOFF);  // alarm tiap 8:1
  Alarm.alarmRepeat(jam2_on, menit2_on, detik2_on, lightON);  // alarm tiap 8:1
  Alarm.alarmRepeat(jam2_off,menit2_off,detik2_off, lightOFF);  // alarm tiap 8:1
  Serial.print("alarm has been set on: ");
  Serial.print(jam_on + " " + menit_on);
  Serial.println();
  Serial.print("alarm has been set on: ");
  Serial.print(jam2_on + " " + menit2_on);
  Serial.println();
  }
  if (actfeed==1){
    Alarm.alarmRepeat(6, 30, 00, feedingGuideLight);
    Alarm.alarmRepeat(12, 30, 00, feedingGuideLight);
    Alarm.alarmRepeat(18, 30, 00, feedingGuideLight);
  }
  
//  while (!eeprom_is_ready());
//  cli();
//  EEPROM.write(addr, actled);
//  sei();
//  while (addr == EEPROM.length()) {
//  addr = addr + 1;
//  if(addr == EEPROM.length())
//    addr = 0;}
  Serial.print("actled is: ");
  Serial.print(actled);
  Serial.println();
}
 void powerfailureRecovery(){
  
        Serial.print("power failure executed ");
       //
        Serial.println();
    if(((hour() == jam_on && minute()> menit_on )  || (hour() > jam_on   ) ) && (actled==1||actled==3)){ //jika waktu sekarang masuk jam dimana led 
      if(hour() == jam_off && minute()< menit_off){                                                        //haurs menyala, maka akan 
        //led_on=1;
        lightON();
        Serial.print("led_on value is: ");
        Serial.print(led_on);
        Serial.println();}
      else if (hour() < jam_off ){ 
        lightON();
        Serial.print("led_on value is: ");
        Serial.print(led_on);
        Serial.println();
        }
       
      else{
      lightOFF();} 
      }
  
  if(((hour() == jam2_on && minute()> menit2_on )  || (hour() > jam2_on ))  && (actled==2||actled==3)){ //jika waktu sekarang masuk jam dimana led 
      
      Serial.println("ok1 ");
      if(hour() == jam2_off && minute()< menit2_off){                                                        //haurs menyala, maka akan 
        //led_on=1;
        
        Serial.println("ok2 ");
        lightON();
        Serial.print("led_on value is: ");
        Serial.print(led_on);
        Serial.println();}
      else if (hour() < jam2_off ){ 
        
        Serial.println("ok3 ");
        lightON();
        Serial.print("led_on value is: ");
        Serial.print(led_on);
        Serial.println();
        }
    }
  
  else
  {
    //led_on=0;
    lightOFF();
    Serial.print("led_on value is: ");
     Serial.print(led_on);
  }
 }
void IRrecvHandle(){
  
  if (irx.decode(&results)) {
    if (results.value == 0xFF30CF) { //power button remot aico
     doubleBEEP();
    //if (results.value == 0x40BFB847) {
     Serial.println("Power button Pressed");
    analogWrite(RELAY_PIN_10, (255));
      delay(200);
    }
    else if (results.value == 0xFF00FF) { //open/close remote aico      
     doubleBEEP();
    //else if (results.value == 0x40BF42BD) {
     Serial.println("Open/close button Pressed");
    analogWrite(RELAY_PIN_10, (0));
      delay(200);
    }
    else if (results.value == 0xFF926D) { //mute remote aico
     doubleBEEP();
    //else if (results.value == 0x40BF42BD) {
     Serial.println("mute button Pressed");
    fishFeeding();
      delay(200);
    }
      else if (results.value == 0xFFB04F) {
     doubleBEEP();
     Serial.println("zoom button Pressed");
    reload();
      delay(200);
    }
      else if (results.value == 0xFF38C7) { //FF aico
     //doubleBEEP();
     Serial.println("enter button Pressed");
    feedingGuideLight();
      delay(200);
    }
    irx.resume(); // Receive the next value
  }
  
  //Serial.print("                            waiting IR signal...... ");
  
    // Serial.println();
}

void lightON(){
    analogWrite(RELAY_PIN_10, (255));
}

void lightOFF(){
    analogWrite(RELAY_PIN_10, (0));
}
void feeder2(){
feeder=1;
  fishFeeding();
}

void fishFeeding(){
  if (feeder==1){
  Serial.print("fish feeding...");
  Serial.println();
  if (feedLIGHT==1){
  digitalWrite(PIN_FEEDER_7, 1);
  delay(100);
//  digitalWrite(PIN_FEEDER_7, 0);
//  delay(10000);
//  digitalWrite(PIN_FEEDER_7, 1);
//  delay(110);
  digitalWrite(PIN_FEEDER_7, 0);
  feedingLIGHT=1;
  feeder=0;
  }

  }
    else {
  digitalWrite(PIN_FEEDER_7, 1);
  delay(100);
  digitalWrite(PIN_FEEDER_7, 0);
  
  }
  }
//      //delay(30);
  //alarm.timerOnce(34200, 


 void feedingSignalLight(){
  if (feedingLIGHT==1){
 // angka3++;
  angka4++;
  angka5++;
   
    if (angka5<500){
        if (angka5<200){
          feedingLIGHT=1;
            if (angka4 ==1 ){digitalWrite(BUZZ_PIN_8, 0);}
            else if (angka4 >10 ){digitalWrite(BUZZ_PIN_8, 1); }
        }
        //else if (angka5==150){fishFeeding();}
        else {digitalWrite(BUZZ_PIN_8, 0);}
       
        if (angka4==1){
          analogWrite(PIN_GfeedLIGHT, (0));
        }
       // else if (angka4 ==2 ){digitalWrite(BUZZ_PIN_8, 0);}
        else if (angka4 >10 ){
          analogWrite(PIN_GfeedLIGHT, (255));
          //digitalWrite(BUZZ_PIN_8, 1);
         angka4=0;
        }
        
    }
    
    else {
      angka5=0;
      digitalWrite(BUZZ_PIN_8, (0));
      feedingLIGHT=0;}
    
  }
//  digitalWrite(PIN_GfeedLIGHT, HIGH);  
//  
//  if (angka3 >= 200){
//    digitalWrite(PIN_GfeedLIGHT, LOW);     
//    angka3 = 0;
//  }
  
  delay(100);
 }
 
 void feedingGuideLight(){
  feedLIGHT=1;
  feedingLight();
 }
 void feedingLight(){
  if (feedLIGHT==1){
 // angka3++;
  angka4++;
  angka5++;
    if (angka5<300){
        if (angka5<300){
          feedingLIGHT=1;
          
            if (angka4 ==1 ){digitalWrite(BUZZ_PIN_8, 0);}
            if (angka4==10){analogWrite(PIN_GfeedLIGHT, (255));}
            if (angka4==11){analogWrite(PIN_GfeedLIGHT, (0));}
            if (angka4 >20 ){digitalWrite(BUZZ_PIN_8, 1);
            analogWrite(PIN_GfeedLIGHT, (255));
            angka4=0;}
        }
        if (angka5==150){feeder2();}
        if (angka5==200){feeder2();}  
        //else {digitalWrite(BUZZ_PIN_8, 0);}
       
        if (angka4==1){
          analogWrite(PIN_GfeedLIGHT, (0));
        }
    }
    else {
      angka5=0;
      digitalWrite(BUZZ_PIN_8, (0));
      feedLIGHT=0;} 
  }
  delay(100);
 }
void digitalClockDisplay(void)
{
  angka++;
  angka2++;
  if (angka >=50){
      
  // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(' ');
    Serial.print(day());
    Serial.print(' ');
    Serial.print(month());
    Serial.print(' ');
    Serial.print(year()); 
    Serial.println(); 
    
  infoDisplay();
  angka=0;
  }
  if (angka2 >= 10){
    Alarm.delay(10);
    angka2=0;
  }
    delay(1);
}

void infoDisplay()
{
  Serial.print("lightON hour : ");
  Serial.print(jam_on);
  Serial.print(" > ");
  Serial.print(hour());
  Serial.print(" < ");
  Serial.print("lightOFF hour : ");
  Serial.print(jam_off);
  Serial.println();
  Serial.print("lightON minute : ");
  Serial.print(menit_on);
  Serial.print(" > ");
  Serial.print("now minute : ");
  Serial.print(minute()); 
  Serial.print(" < ");
  Serial.print("lightOFF minute : ");
  Serial.print(menit_off);
  Serial.println();
     Serial.print(" ");
  Serial.println();
  
  Serial.print("lightON hour2 : ");
  Serial.print(jam2_on);
  Serial.print(" > ");
  Serial.print(hour());
  Serial.print(" < ");
  Serial.print("lightOFF hour2 : ");
  Serial.print(jam2_off);
  Serial.println();
  Serial.print("lightON minute2 : ");
  Serial.print(menit2_on);
  Serial.print(" > ");
  Serial.print("now minute : ");
  Serial.print(minute()); 
  Serial.print(" < ");
  Serial.print("lightOFF minute2 : ");
  Serial.print(menit2_off);
  Serial.println();
     Serial.print(" ");
  Serial.println();
 // delay(1000);
}

void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(':');
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}
