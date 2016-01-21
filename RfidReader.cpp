/*
* RfidReader.cpp
* 
* The MIT License (MIT)
* Copyright (c) 2016 Pär Stigenberg
*/

#include "RfidReader.h"

/*
* RC522RfidReader
* 
*/
bool RC522RfidReader::SetupReader(){
  mfrc522 = new MFRC522(10, 9);

  // Debug
  Serial.begin(9600);
  
  SPI.begin();            // Init SPI bus
  mfrc522->PCD_Init();    // Init MFRC522
  
  // Get the MFRC522 software version
  byte v = mfrc522->PCD_ReadRegister(mfrc522->VersionReg);
  
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    return false;
  }

  Serial.println("Reader setup ok");
  return true;
}

RfidTag RC522RfidReader::GetRfidTag() {
  return rfidTag;
}

bool RC522RfidReader::ReadNextRfidTag() { 
  bool success = false;
  while(!success) {
    success = ReadRfidTag();
  }
  return true;    
}

bool RC522RfidReader::ReadNewRfidTag(int timeoutMS) {
  
  unsigned long startTimeStamp = millis(); // Start timer
  RfidTag lastReadRfidTag = rfidTag;
  
  while(true) {
    
    // Check if time out
    if(Timer::TimeOut(startTimeStamp, millis(), timeoutMS))
      return false;
    
    // Try to read tag  
    if (ReadRfidTag()) {
      if (memcmp(rfidTag.GetRfidTag(), lastReadRfidTag.GetRfidTag(), 10) != 0) {
          return true;  
      }
    }     
    
  } // End while loop
  
}

bool RC522RfidReader::ReadRfidTag() {
  // Look for new cards

    //Serial.println("Looking for card");
    
    if (!mfrc522->PICC_IsNewCardPresent()) {
      return false;
    } 

    Serial.println("New card found");
  
    // Select one of the cards
    if (!mfrc522->PICC_ReadCardSerial()) {
      return false;
    }

    Serial.println("New card read");

    // We got a card
    byte readCard[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int uidCounter = 0;
    for (int i=10-mfrc522->uid.size; i<10; i++){
      readCard[i] = mfrc522->uid.uidByte[uidCounter];
      uidCounter++;
    }

    rfidTag.SetTag(readCard);
    return true;
}

/*
* RDM630RfidReader
* 
*/
bool RDM630RfidReader::SetupReader(){
/*
  rfidReader = new rdm630(10,0);
  rfidReader->begin();
*/
  return true;
}

RfidTag RDM630RfidReader::GetRfidTag() {
  return rfidTag;
}

bool RDM630RfidReader::ReadNextRfidTag() { 
  return ReadRfidTag();  
}

bool RDM630RfidReader::ReadNewRfidTag(int timeoutMS) {
  return false;
  /*
  unsigned long startTimeStamp = millis(); // Start timer
  RfidTag lastReadRfidTag = rfidTag;
  
  while(true) {
    
    // Check if time out
    if(Timer::TimeOut(startTimeStamp, millis(), timeoutMS))
      return false;
    
    // Try to read tag  
    if (ReadRfidTag()) {
      if (memcmp(rfidTag.GetRfidTag(), lastReadRfidTag.GetRfidTag(), 10) != 0) {
          return true;  
      }
    }     
    
  } // End while loop
  */ 
}

bool RDM630RfidReader::ReadRfidTag() {
  /*
  byte data[6];
  byte len;

  if (rfidReader->available()) {
    Serial.println("New card found");
    rfidReader->getData(data,len);

    // We got a card
    byte readCard[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    int uidCounter = 0;
    for (int i=10-len; i<10; i++){
      readCard[i] = data[uidCounter];
      uidCounter++;
    }

    rfidTag.SetTag(readCard);
    return true;
  } 
  */
  return false;
}

