/*
 * 
 * RfidReader
*/

#ifndef RfidReader_h
#define RfidReader_h

#include <SPI.h>


#include "Arduino.h"
#include "RfidTagHandler.h"
#include "MFRC522.h"

//#include "rdm630.h"

class RfidReader
{
  public:
    
    // Setup communication with reader
    virtual bool SetupReader() = 0;
    
    // Tries to read next RFID tag, return true if successful. 
    virtual bool ReadNextRfidTag() = 0;
    
    // Waits for a new tag, returns true if successful, false if no new card found within timeout
    virtual bool ReadNewRfidTag(int timeoutMS = 5000) = 0;
    
    // Returns the RFID tag
    virtual RfidTag GetRfidTag() = 0;

  protected:
    RfidTag rfidTag;
};

class RC522RfidReader : public RfidReader
{
  public:
    virtual bool SetupReader();
    virtual bool ReadNextRfidTag();
    virtual bool ReadNewRfidTag(int timeoutMS);
    virtual RfidTag GetRfidTag();
     
  private:
    MFRC522 *mfrc522 = NULL;
    bool ReadRfidTag();
};

class RDM630RfidReader : public RfidReader
{
  public:
    virtual bool SetupReader();
    virtual bool ReadNextRfidTag();
    virtual bool ReadNewRfidTag(int timeoutMS);
    virtual RfidTag GetRfidTag();

  private:
    //rdm630 *rfidReader = NULL;
    bool ReadRfidTag();
};

class RfidReaderFactory
{
  public:
  
    static RfidReader *GetInstance(String reader) {
        if (reader == "RC522")
          return new RC522RfidReader;
        //else
        //  return new RDM630RfidReader; 
     }
};

class Timer
{
  public:
    static bool TimeOut(unsigned long startTimeStamp, unsigned long currentTimeStamp, unsigned long timeoutMS) {
        if(currentTimeStamp >= startTimeStamp) {
          unsigned long difference = currentTimeStamp - startTimeStamp;
          if(difference > 5000)
            return true;
          } else if(startTimeStamp > 5000) {
            return true;
          }
      
        return false;
  }
};

#endif
