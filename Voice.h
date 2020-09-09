#ifndef VOISE_H
#define VOISE_H

#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>

#define stopCom (0)
#define forwardCom (1) 
#define backwardCom (2)
#define leftCom (3)
#define rightCom (4)

class Voice
{
public:
  Voice();
  void init();
  int loopFunc();
  void timerIsr();
  void printVR(uint8_t *buf);
  void printSignature(uint8_t *buf, int len);

private:
  uint8_t records[7];
  uint8_t buf[64];
  int led = 13;

  VR *myVR;
};

Voice::Voice() 
{
  Serial.println("Elechouse Voice Recognition V3 Module\r\nControl LED sample");
}

void Voice::init()
{
  myVR = new VR(2, 3);
  myVR->begin(9600);
  pinMode(led, OUTPUT);
  
  if (myVR->clear() == 0)
    Serial.println("Recognizer cleared.");
  else
  {
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR->load((uint8_t)stopCom) >= 0)
    Serial.println("Stop: loaded");
    
  if(myVR->load((uint8_t)forwardCom) >= 0)
    Serial.println("Forward: loaded");

  if(myVR->load((uint8_t)backwardCom) >= 0)
    Serial.println("Backward: loaded");

  if(myVR->load((uint8_t)leftCom) >= 0)
    Serial.println("Left: loaded");

  if(myVR->load((uint8_t)rightCom) >= 0)
    Serial.println("Right: loaded");
}

int Voice::loopFunc()
{
  int ret;
  ret = myVR->recognize(buf, 50);
  if(ret > 0)
  {
    if (ret > 4)
      ret = ret - 4;

    printVR(buf);
    return ret;
  }  
  return -2;
}

void Voice::printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");
  Serial.print(buf[2], DEC);
  Serial.print("\t\t");
  
  if (buf[0] == 0xFF)
      Serial.print("NONE");
  else if (buf[0]&0x80) 
  {
      Serial.print("UG ");
      Serial.print(buf[0]&(~0x80), DEC);
  } else 
  {
      Serial.print("SG ");
      Serial.print(buf[0], DEC);
  }
  Serial.print("\t");
  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  
  if (buf[3] > 0)
      printSignature(buf+4, buf[3]);
  else
      Serial.print("NONE");
  
  Serial.println("\r\n");
}

void Voice::printSignature(uint8_t *buf, int len)
{
  int i;
  for(i = 0; i < len; i++) 
  {
      if (buf[i]>0x19 && buf[i]<0x7F)
          Serial.write(buf[i]);
      else 
      {
          Serial.print("[");
          Serial.print(buf[i], HEX);
          Serial.print("]");
      }
  }
}

void Voice::timerIsr()
{
  digitalWrite(13, digitalRead(13) ^ 1);
}

#endif
