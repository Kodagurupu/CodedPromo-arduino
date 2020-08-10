#ifndef LOGIC_H
#define LOGIC_H

#include "Stepper.h"
#include <SoftwareSerial.h>

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
    int step_zero,
    int step_first,
    int step_second,
    int step_third,
    int ultrasonic[19]) 
    : leftControll(lControll),
      rightControll(rControll),
      bluetoothPin0(bPin0),
      bluetoothPin1(bPin1),
      bluetooth(bPin0, bPin1),
      currentStep(0)
  {
    movement = new MyStepper( 
        step_zero, 
        step_first, 
        step_second, 
        step_third,
        STEPPERMIN,
        STEPPERMAX
    );
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
      if (recivedData == 'f') goFoward();
      if (recivedData == 'l') turnLeft(1);
      if (recivedData == 'r') turnRight(1);
      if (recivedData == 's') stopMoving();
    }
  }

  void stopMoving() 
  {
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

  void goFoward() 
  {
    if (globalRules()) 
    {
      while (currentStep != STEPPERMAX) 
      {
        movement->goStep(2);
        currentStep += 5;
        if ( !globalRules() ) 
        {
          setStatic();
          break;
        }
        delay(5);
      }
    }
  }

  void goBack()
  {
    if (globalRules()) 
    {
      while (currentStep != STEPPERMIN) 
      {
        movement->goStep(-2); 
        if ( !globalRules() ) 
        {
          setStatic();
          return;
        }
        currentStep -= 5;
      }
    }
  }

  void setStatic()
  {
    Serial.println("SU");
    Serial.println(currentStep);
    while (currentStep != 0) 
    {
      Serial.println(currentStep);
      movement->msetSpeed(200);
      if (currentStep < 0) 
      {
        movement->goStep(2);
        currentStep += 5;
      }
      else if (currentStep > 0) 
      {
        movement->goStep(-2);
        currentStep -=5;
      }
    }
  }

private:

  char recivedData;
  
  int leftControll;
  int rightControll;
  int steppControll;
  int bluetoothPin0;
  int bluetoothPin1;
  int currentStep;

  int ultraSonicPins[19];

  bool turning;
  bool goingFoward;

  MyStepper *movement;
  SoftwareSerial bluetooth;
};

#endif
