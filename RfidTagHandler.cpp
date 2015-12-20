/*
 * 
 * RfidTagHandler
*/

#include "RfidTagHandler.h"

/*
 * TagListHandler
*/

TagListHandler::TagListHandler() {
  ReadList();  
}

int TagListHandler::GetValidTagListCount(){
  return validTagListCount;
}

bool TagListHandler::IsTagValid(RfidTag rfidTag) {
  for (int i=0; i<ALLOWED_TAGS_COUNT; i++) {
    if (memcmp(rfidTag.GetRfidTag(), validTagList[i], 10) == 0) {
      return true;
    }
  }
  return false; 
}

bool TagListHandler::AddTag(RfidTag rfidTag) {
  
  if( validTagListCount >= ALLOWED_TAGS_COUNT ||  // Memory full
      IsTagValid(rfidTag) ||                      // Already in the list
      rfidTag.IsAddTag() ||                       // Cant add admin card
      rfidTag.IsDeleteTag()) {                    // Cant add admin card
        return false;
      }

  // Add card to list
  for (int i=0; i<10; i++)
    validTagList[validTagListCount][i] = rfidTag.GetRfidTag()[i];

  //GetRfidTag()
  return SaveList();
}

bool TagListHandler::DeleteTag(RfidTag rfidTag){
    // Find index
  int cardIndex = -1;
  for (int i=0; i<ALLOWED_TAGS_COUNT; i++) {
    if (memcmp(rfidTag.GetRfidTag(), validTagList[i], 10) == 0) {
      cardIndex = i;
    }
  }

  // If card dont exist, reurn ok
  if (cardIndex < 0) 
    return true;

  // Remove card
  for (int i=0; i<10; i++)
    validTagList[cardIndex][i] = 0x00;

  return SaveList();  
}

bool TagListHandler::ReadList(){
  bool success = false;
  validTagListCount = 0;
  while (validTagListCount<ALLOWED_TAGS_COUNT) {
    byte readBuffer[10];
    success = EepromUtil::eeprom_read_bytes(validTagListCount*10, readBuffer, 10);
    if (success && 
        memcmp(EMPTY_TAG, readBuffer, 10) != 0 && 
        memcmp(EMPTY_MEMORY, readBuffer, 10) != 0) {
          // Add card to valid list
          memcpy(validTagList[validTagListCount], readBuffer, 10);
    } else {
      break;
    }
    validTagListCount++;
  }
  return success;
}

bool TagListHandler::SaveList(){
  EraseStorage();
  int currentAddres = 0;
  for (int i=0; i<ALLOWED_TAGS_COUNT; i++) {
    if (memcmp(EMPTY_TAG, validTagList[i], 10) != 0 ) {
      if(EepromUtil::eeprom_write_bytes(currentAddres, validTagList[i], 10)){
        currentAddres += 10;
      }
    }
  }

  ResetMemory();          // Reset list of valid cards
  return ReadList(); // Read valid cards from EEPROM
}

void TagListHandler::ResetMemory(){
  for (int i=0; i<ALLOWED_TAGS_COUNT; i++)
    for (int j=0; j<10; j++)
      validTagList[i][j] = 0x00;
}

bool TagListHandler::EraseStorage(){
  for (int i=0; i<ALLOWED_TAGS_COUNT; i++) {
    if (!EepromUtil::eeprom_write_bytes(i*10, EMPTY_TAG, 10)){
      return false;
    }
  }
  return true;
}

