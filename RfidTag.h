/*
 * 
 * RfidTag
*/

#ifndef RfidTag_h
#define RfidTag_h

#include "Arduino.h"

class RfidTag
{
  public:

    RfidTag();
    
    byte* GetRfidTag();
    int GetTagLength();
    
    void SetTag(byte tag[10]);
    
    bool IsAddTag();
    bool IsDeleteTag();
    bool IsAdminTag();

    private:
    int tagLength;
    byte rfidTag[10] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
};

#endif


