/*
 * RfidTag.cpp
* 
* The MIT License (MIT)
* Copyright (c) 2016 Pär Stigenberg
*/


#include "RfidTag.h"

// Admin cards
const int ADD_TAG_COUNT = 1;
byte ADD_TAG_LIST[ADD_TAG_COUNT][10] =  {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0x21, 0x54, 0x07} // 4B 21 54 07 
};

// Delete cards
const int DELETE_TAG_COUNT = 1;
byte DELETE_TAG_LIST[DELETE_TAG_COUNT][10] =  {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4B, 0xE3, 0x52, 0x07} // 4B E3 52 07
};

RfidTag::RfidTag() {}

void RfidTag::SetTag(byte tag[10]) {

  for (int i=0; i<10; i++){
    rfidTag[i] = tag[i];
  }

  int counter = 0;
  for (int i=0; i<10; i++){
    if(rfidTag[i] == 0x00)
      counter++;
    else
      break;
  }

  tagLength = 10 - counter;
}

byte* RfidTag::GetRfidTag() {
  return rfidTag;
}

int RfidTag::GetTagLength() {
  return tagLength;  
}

bool RfidTag::IsAddTag() {
  for (int i=0; i<ADD_TAG_COUNT; i++) {
    if(memcmp(rfidTag, ADD_TAG_LIST[i], 10) == 0)
      return true;
  }
  return false;
}

bool RfidTag::IsDeleteTag() {
  for (int i=0; i<DELETE_TAG_COUNT; i++) {
    if(memcmp(rfidTag, DELETE_TAG_LIST[i], 10) == 0)
      return true;
  }
  return false;
}

bool RfidTag::IsAdminTag(){
  if (IsAddTag() || IsDeleteTag()){
    return true;  
  }
  return false;
}


