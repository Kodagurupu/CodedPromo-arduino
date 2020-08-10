#ifndef LOGIC_H
#define LOGIC_H

#include "MovingLib.h"

#define DEBUGMODE true
#define ULTRASONICDELAY_LOW 3
#define ULTRASONICDELAY_HIGH 15

class Logic 
{  

public:

  Logic::Logic(
    int pin1, 
    int pin2,
    int pin3,
    int pin4,
    int ultrasonic[19]
  ) 
    : moving(
        pin1,
        pin2,
        pin3,
        pin4
      )
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
    
    for ( int i = 0; i < 20; i++ ) 
    {
      if (ultraSonicPins[i] != 0) 
      {
        if ( i % 2 == 0 ) pinMode(ultraSonicPins[i], OUTPUT);
        else pinMode(ultraSonicPins[i], INPUT);
      }      
    }
    delay(500);
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

  void ReadSerial() 
  {
    if ( Serial.available() > 0 )
    {
      int command = Serial.parseInt();
      if ( command == -1 ) stopMoving();
      if ( command == 0 ) goForward();
      if ( command == 1 ) goBackward();
      if ( command == 2 ) turnLeft();
      if ( command == 3 ) turnRight();
    }
  }

  void stopMoving() 
  {
    if (DEBUGMODE) Serial.println("stoping");
    moving.restore();
  }

  void turnLeft() 
  {
    if (DEBUGMODE) Serial.println("Turning left");
    moving.turnLeft();  
  }

  void turnRight() 
  {
    if (DEBUGMODE) Serial.println("Turning right");
    moving.turnRight();
  }

  void goForward() 
  {
    if (DEBUGMODE) Serial.println("Moving forward");
    moving.goFoward();
  }

  void goBackward()
  {
    if (DEBUGMODE) Serial.println("Moving backward");
    moving.goBackward();
  }

private:

  Moving moving;
  int ultraSonicPins[19];
  
};

#endif
