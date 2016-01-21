/*
* RfidMagneticLockApp.ino
* 
* The MIT License (MIT)
* Copyright (c) 2016 Pär Stigenberg
*/

#include "RfidTagHandler.h"
#include "RfidReader.h"
#include "RfidTag.h"

// LEDS
const int RGB_RED_PIN = 6;
const int RGB_GREEN_PIN = 8;
const int RGB_BLUE_PIN = 5;
const int OPEN_LOCK_PIN = 4;

const int LONG_BLINK = 400;
const int SHORT_BLINK = 200;

// DEBUG
const bool DEBUG_MODE = false;

TagListHandler tagListHandler;
RfidReader *rfidReader;


void setup() {
  pinMode(RGB_RED_PIN, OUTPUT);
  pinMode(RGB_GREEN_PIN, OUTPUT);
  pinMode(RGB_BLUE_PIN, OUTPUT);
  pinMode(OPEN_LOCK_PIN, OUTPUT);

/*
  if(DEBUG_MODE) {
    
    Bridge.begin();
    Console.begin(); 
  
    while (!Console); 
      PrintDebugLine("Console connected!");
   
  }
  */

  rfidReader = RfidReaderFactory::GetInstance("RC522");
  if (rfidReader->SetupReader()){
    GreenBlink(SHORT_BLINK);  
  } else {
    RedBlink(SHORT_BLINK);
  }

  delay(1000);
    
  GreenBlink(SHORT_BLINK);
  RedBlink(SHORT_BLINK);
  BlueBlink(SHORT_BLINK);
  WhiteBlink(SHORT_BLINK);
}

void loop() {

  if (!rfidReader->ReadNextRfidTag()) {
    return;
  }

  if (rfidReader->GetRfidTag().IsAddTag()){
    WhiteBlink(SHORT_BLINK);
    bool success = rfidReader->ReadNewRfidTag();
    if (success) {
      if (rfidReader->GetRfidTag().IsAdminTag()) {
        WhiteBlink(SHORT_BLINK, 2); // Error, not allowed
      } else if (tagListHandler.AddTag(rfidReader->GetRfidTag())) {
        WhiteBlink(SHORT_BLINK); // Ok
      } else {
        WhiteBlink(SHORT_BLINK, 2); // Error saving card
      }
    } else {
      WhiteBlink(SHORT_BLINK, 3); // Time out
    }    
  } else if (rfidReader->GetRfidTag().IsDeleteTag()) {
    BlueBlink(SHORT_BLINK);
    bool success = rfidReader->ReadNewRfidTag();
    if (success) {
      if (rfidReader->GetRfidTag().IsAdminTag()) {
        BlueBlink(SHORT_BLINK, 2); // Error, not allowed
      } else if (tagListHandler.DeleteTag(rfidReader->GetRfidTag())) {
        BlueBlink(SHORT_BLINK); // Ok
      } else {
        BlueBlink(SHORT_BLINK, 2); // Error saving card
      }
    } else {
      BlueBlink(SHORT_BLINK, 3); // Time out
    } 
  } else if (tagListHandler.IsTagValid(rfidReader->GetRfidTag())) {
    GreenBlink(SHORT_BLINK);
    OpenLock(); 
  } else {
    RedBlink(SHORT_BLINK);  
  }
}

void OpenLock() {
  digitalWrite(OPEN_LOCK_PIN, HIGH);
  delay(2000);
  digitalWrite(OPEN_LOCK_PIN, LOW); 
}

/*
 * Debug led functions
 */

void ResetAllLeds() {
  digitalWrite(RGB_RED_PIN, 0);
  digitalWrite(RGB_GREEN_PIN, 0);
  digitalWrite(RGB_BLUE_PIN, 0);
}
void GreenBlink(int delayMS){
    
  ResetAllLeds();
  digitalWrite(RGB_GREEN_PIN, 255);
  delay(delayMS);
  ResetAllLeds();
}

void RedBlink(int delayMS){
  ResetAllLeds();
  digitalWrite(RGB_RED_PIN, 255);
  delay(delayMS);
  ResetAllLeds();  
}

void BlueBlink(int timeLitMS, int iterations){
  for (int i=0; i<iterations; i++) {
    BlueBlink(timeLitMS);
    delay(timeLitMS);
  }
}

void BlueBlink(int delayMS){
  ResetAllLeds();  
  digitalWrite(RGB_BLUE_PIN, 255);  
  delay(delayMS);
  ResetAllLeds();
}

void WhiteBlink(int timeLitMS, int iterations){
  for (int i=0; i<iterations; i++) {
    WhiteBlink(timeLitMS);
    delay(timeLitMS);
  }
}

void WhiteBlink(int timeLitMS){
    ResetAllLeds();  
    digitalWrite(RGB_RED_PIN, 255);
    digitalWrite(RGB_GREEN_PIN, 255);
    digitalWrite(RGB_BLUE_PIN, 255);  
    delay(timeLitMS);
    ResetAllLeds();
}


/*
 * Print functions 
 */

/*

void PrintMemory(){
  if (!DEBUG_MODE)
    return;
    
  Console.print("Free memory: ");
  Console.println(freeMemory());
}

void PrintCardData(RfidTag rfidTag){
  if (!DEBUG_MODE)
    return;
    
  PrintCardData("", rfidTag);
}

void PrintCardData(String msg, RfidTag rfidTag){
  if (!DEBUG_MODE)
    return;
    
  if(msg != "") {
    Console.print(msg);
    Console.print(" - ");  
  }
  
  // Display card data
  Console.print("Card: ");
  for (int i = 0; i < 10; i++) {  
    Console.print(rfidTag.GetRfidTag()[i], DEC);
    Console.print(" ");
  }
  Console.println("");
}

void PrintDebugLine(String msg){
  if (!DEBUG_MODE)
    return;
  
  Console.println(msg);
}

*/

