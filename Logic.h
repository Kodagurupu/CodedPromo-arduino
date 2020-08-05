#ifndef LOGIC_H
#define LOGIC_H

#include "ServoLib.h"
#include <SoftwareSerial.h>

#define DEBUGMODE true
#define STEPPERMAX 1000
#define STEPPERMIN -1000
#define ULTRASONICDELAY_LOW 3
#define ULTRASONICDELAY_HIGH 15

class Logic 
{  
public:

  Logic::Logic(
    int lControll, 
    int rControll, 
    int bPin0, 
    int bPin1,
    int servo0,
    int servo1,
    int ultrasonic[19]
  ) 
    : leftControll(lControll),
      rightControll(rControll),
      bluetoothPin0(bPin0),
      bluetoothPin1(bPin1),
      bluetooth(bPin0, bPin1),
      servoPin0(servo0),
      servoPin1(servo1)
  {
    // init ultrasonic pins
    for ( int i = 0; i < 20; i++ ) 
    {
      if (ultrasonic[i] != 0) 
        ultraSonicPins[i] = ultrasonic[i];
      else ultraSonicPins[i] = 0;      
    } 
  }

  void init() 
  {    
    //Init Seral
    
    Serial.begin(9600);
    Serial.println("[CORE] Initializing");  
    
    servoLibrary = new ServoLib(
      servoPin0,
      servoPin1  
    );
    
    for ( int i = 0; i < 20; i++ ) 
    {
      if (ultraSonicPins[i] != 0) 
      {
        if ( i % 2 == 0 ) pinMode(ultraSonicPins[i], OUTPUT);
        else pinMode(ultraSonicPins[i], INPUT);
      }      
    } 
    // Configure pins
    pinMode(leftControll,  OUTPUT);
    pinMode(rightControll, OUTPUT);
    digitalWrite(leftControll, LOW);
    digitalWrite(rightControll, LOW);
    delay(1000);
  }

  void ultraSonicBlink(int i)
  {
    digitalWrite(ultraSonicPins[i], LOW);
    delayMicroseconds(ULTRASONICDELAY_LOW);
    digitalWrite(ultraSonicPins[i], HIGH);
    delayMicroseconds(ULTRASONICDELAY_HIGH);
    digitalWrite(ultraSonicPins[i], LOW);
  }

  bool globalRules() 
  {
    return true;
    for (int i = 0; i < 19; i++) 
    {
      if (ultraSonicPins[i] != 0 && i % 2 == 0) 
      {
        long duration0, duration1, distance;
        ultraSonicBlink(i);
        duration0 = pulseIn(ultraSonicPins[i+1], HIGH);
        ultraSonicBlink(i);
        duration1 = pulseIn(ultraSonicPins[i+1], HIGH);
        distance = ((duration0 + duration1) / 2) * 0.034/2;
        if (distance < 10) return false;
      }
    }
    return true;
  }

  void ReadBluetooth() 
  {
    if (bluetooth.available())
    {
      recivedData = bluetooth.read();
      recivedData = Serial.read();
      if (DEBUGMODE) Serial.println("Reived: " + recivedData);
      if (recivedData == 'f') goFoward();
      if (recivedData == 'l') turnLeft(1);
      if (recivedData == 'r') turnRight(1);
      if (recivedData == 's') stopMoving();
    }
    if (recivedData == "0")
    {
      Serial.print("Recived: ");
      Serial.println(recivedData);
      for (int i = 0; i < 2; i++)
      {
        digitalWrite(13, HIGH);
        delay(500);
        digitalWrite(13, LOW);
      }
    }
  }

  void stopMoving() 
  {
    if (DEBUGMODE) Serial.println("stoping");
    turning = false;
    digitalWrite(leftControll, LOW);
    digitalWrite(rightControll, LOW);
    goBack();
    setStatic();
  }

  void turnLeft(int delaySec) 
  {
    if (!turning) 
    {
      if (DEBUGMODE) Serial.println("turning");
      toggleTurn();
      digitalWrite(leftControll, HIGH);
      digitalWrite(rightControll, HIGH);
      if ( !globalRules() ) 
      {
        digitalWrite(leftControll, LOW);
        digitalWrite(rightControll, LOW);
        toggleTurn();
        setStatic();
        return;
      }
      delay(delaySec * 100);
      if ( !globalRules() ) 
      {
        digitalWrite(leftControll, LOW);
        digitalWrite(rightControll, LOW);
        toggleTurn();
        setStatic();
        return;
      }
      digitalWrite(leftControll, LOW);
      digitalWrite(rightControll, LOW);
      toggleTurn();
    }
  }

  void turnRight(int delaySec) 
  {
    if (!turning) 
    {
      if (DEBUGMODE) Serial.println("turning");
      toggleTurn();
      digitalWrite(leftControll, HIGH);
      if ( !globalRules() ) 
      {
        digitalWrite(leftControll, LOW);
        toggleTurn();
        setStatic();
        return;
      }
      delay(delaySec * 100);
      if ( !globalRules() ) 
      {
        digitalWrite(leftControll, LOW);
        toggleTurn();
        setStatic();
        return;
      }
      digitalWrite(leftControll, LOW);
      toggleTurn();
    }
  }

  void toggleTurn() 
  {
    turning = !turning;
  }

  void check()
  {
  }

  void goFoward() 
  {
    servoLibrary->goFoward();
  }

  void goBack()
  {
    servoLibrary->goBackward();
  }

  void setStatic()
  {
  }

private:

  char recivedData;

  int servoPin0;
  int servoPin1;
  int leftControll;
  int rightControll;
  int steppControll;
  int bluetoothPin0;
  int bluetoothPin1;
  int currentStep;

  int ultraSonicPins[19];

  bool turning;
  bool goingFoward;

  ServoLib *servoLibrary;
  SoftwareSerial bluetooth;
};

#endif
