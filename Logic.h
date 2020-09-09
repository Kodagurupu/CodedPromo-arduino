#ifndef LOGIC_H
#define LOGIC_H

#include "Moving.h"
#include "Voice.h"
#include "Ultrasonic.h"

#define DEBUGMODE false

enum MovingMode
{
  Default = 0,
  Walking = 1,
  HumanControl = 2
};

class Logic 
{  
public:
  Logic(
    short unsigned int pin1, 
    short unsigned int pin2,
    short unsigned int pin3,
    short unsigned int pin4,
    Module devices[4]
  );
  void init();
  void loopFunc();
  void sendCommand(short unsigned int command);
  void readExternalControls();
  void stopMoving();
  void moveLeft();
  void moveRight();
  void goForward();
  void goBackward();
  void toggleMode(MovingMode mode);
  Module devices_[4];

private:
  
  
  Voice voice;
  Moving moving;
  MovingMode _mode;
  Ultrasonic ultrasonic;
};

Logic::Logic (
  short unsigned int pin1, 
  short unsigned int pin2,
  short unsigned int pin3,
  short unsigned int pin4,
  Module devices[4]
) 
  :
    moving(
      pin1,
      pin2,
      pin3,
      pin4
    )
{ 
  for (int i = 0; i < 4; i++)
  {
    devices_[i] = devices[i]; 
  }
}

void Logic::loopFunc()
{
  if (_mode == Default)
    readExternalControls();
  if (_mode == Walking)
  {
    Serial.println(ultrasonic.canMove());
    while(ultrasonic.canMove())
      goForward();
    moving.turnLeft();
    if (ultrasonic.canMove())
    {
      moving.turnLeft();
      moving.turnLeft();

       if (!ultrasonic.canMove())
       moving.turnRight();
    }
  }
  // Will add soon
  if (_mode == HumanControl)
    return;
}

void Logic::init()
{
  //Init Seral
  
  Serial.begin(9600);
  Serial.println("[CORE] Initializing");  
  Serial.println("This world is f*cking hard");

  for (int i = 0; i < sizeof(devices_) / sizeof(Module); i++)
    ultrasonic.addDevice(devices_[i]);

  //voice.init();
  delay(500);
}

void Logic::sendCommand(short unsigned int command)
{
  if ( command == -1 ) stopMoving();
  if ( command == 0 ) goForward();
  if ( command == 1 ) goBackward();
  if ( command == 2 ) moveLeft();
  if ( command == 3 ) moveRight();
}

void Logic::readExternalControls()
{
  // Read com port from laptop
  if ( Serial.available() > 0 )
  {
    int command = Serial.parseInt();
    sendCommand(command);
  }
  // Read voice recognation module
  short unsigned int command = voice.loopFunc();
  sendCommand(command - 1);
}

void Logic::stopMoving()
{
  if (DEBUGMODE) Serial.println("stoping");
  moving.restore();
}

void Logic::moveLeft()
{
  if (DEBUGMODE) Serial.println("Turning left");
  if (ultrasonic.canMove())
    moving.moveLeft();  
}

void Logic::moveRight()
{
  if (DEBUGMODE) Serial.println("Turning right");
  if (ultrasonic.canMove())
    moving.moveRight();
}

void Logic::goForward()
{
  if (DEBUGMODE) Serial.println("Moving forward");
  if (ultrasonic.canMove())
    moving.goFoward();
}

void Logic::goBackward()
{
  if (DEBUGMODE) Serial.println("Moving backward");
  if (ultrasonic.canMove(BACK))
    moving.goBackward();
}

void Logic::toggleMode(MovingMode mode)
{
  if (_mode == mode)
    return;
  _mode = mode;
}

#endif
