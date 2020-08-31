#ifndef VOISE_H
#define VOISE_H

#include <SoftwareSerial.h>
#include <VoiceRecognitionV3.h>

#define onRecord (0)
#define offRecord (1) 
#define onRecord1 (2)
#define offRecord1 (3)

class Voice
{
public:
  Voice();
  void init();
  void loopFunc();
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
  if(myVR->load((uint8_t)onRecord) &gt;= 0)
    Serial.println("onRecord loaded");
  if(myVR->load((uint8_t)offRecord) &gt;= 0)
    Serial.println("offRecord loaded");
  if(myVR->load((uint8_t)onRecord1) &gt;= 0)
    Serial.println("onRecord1 loaded");
  if(myVR->load((uint8_t)offRecord1) &gt;= 0)
    Serial.println("offRecord1 loaded");
}

void Voice::loopFunc()
{
  int ret;
  ret = myVR->recognize(buf, 50);
  if(ret&gt;0)
  {
    switch(buf[1]){
      case onRecord:
        /** turn on LED */
        digitalWrite(led, HIGH);
      break;
      case offRecord:
      /** turn off LED*/
        digitalWrite(led, LOW);
      break;
      case onRecord1:
      /** turn on LED */
        digitalWrite(led1, HIGH);
      break;
      case offRecord1:
      /** turn off LED*/
        digitalWrite(led1, LOW);
      break;
      default:
        Serial.println("Record function undefined");
      break;
    }
    /** voice recognized */
    printVR(buf);
  }  
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
