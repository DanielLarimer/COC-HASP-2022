#include "HASP2022_Downlink.h"
#include "HASP2022_SDCard.h"
#include "HASP2022_Temp.h"

extern long ElapsedSeconds;
extern int DownlinkInterval;
extern int FilesNum;
extern int PMTHits;
extern bool SDOpenStatus;
extern bool CanSendDownlink;

void CheckDownlink() {
  if (((ElapsedSeconds % DownlinkInterval) == 0) && (!CanSendDownlink))
  {
    String data = SDOpenStatus;
    data += ", ";
    data += ByteString(FilesNum, 4);
    data += ", ";
    data += ByteString(PMTHits, 8);
    data += ", ";
    data += ByteString(ReadTemp(1), 4);
    data += ", ";
    data += ByteString(ReadTemp(0), 4);
    data += "\n";

    SendDownlink(data);
    PMTHits = 0;
    CanSendDownlink = true;
  }
  else if ((ElapsedSeconds % DownlinkInterval) != 0)
  {
    CanSendDownlink = false;
  }
}

void SendDownlink(String data) {
  Serial.print("DOWNLINK: " + data);
  Serial5.print(data);
}

String ByteString(int num, int bytes) {
  String numString = String(num);
  int len = numString.length();

  if (len > bytes)
  {
    numString = "";
    for (int i = 0; i < bytes; i++)
      numString += "9";

    return numString;
  }
  for (int i = 0; i < (bytes - len); i++)
    numString = "0" + numString;

  return numString;
}
