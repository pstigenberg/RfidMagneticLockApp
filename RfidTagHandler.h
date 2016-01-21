/*
 * RfidTagHandler.h
* 
* The MIT License (MIT)
* Copyright (c) 2016 Pär Stigenberg
*/

#ifndef RfidTagHandler_h
#define RfidTagHandler_h

#include "Arduino.h"
#include "RfidTag.h"
#include "EepromUtil.h"

// Global lists of cards
const int ALLOWED_TAGS_COUNT = 20;
const byte EMPTY_MEMORY[10] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
const byte EMPTY_TAG[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

class TagListHandler
{
  public:
  
    TagListHandler();
    bool IsTagValid(RfidTag rfidTag);
    bool AddTag(RfidTag rfidTag);
    bool DeleteTag(RfidTag rfidTag);

    //byte*[10] GetValidTagList();
    int GetValidTagListCount();
  
  private:
    byte validTagList[ALLOWED_TAGS_COUNT][10];
    int validTagListCount = 0;

    bool ReadList();
    bool SaveList();
    bool EraseStorage();
    void ResetMemory();
  
};

#endif
