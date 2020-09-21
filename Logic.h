#ifndef LOGIC_H
#define LOGIC_H

// Including supporting libs
#include "Moving.h"
#include "Voice.h"
#include "Ultrasonic.h"

// Make true for view debug messages
#define DEBUGMODE false

/*
 * Thre are few ways to controll this robot
 * By default robot will catch signals from outside like serrial input, radio input etc.
 * In walking mode robot will go around obstructions but you also can controll it by radio
 * At human controll mode robot will stay for radio signals and ingone another
 */
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
) : moving(
      pin1,
      pin2,
      pin3,
      pin4
    )
{ 
  // Adding devices to list in extern lib
  for (int i = 0; i < sizeof(devices) / sizeof(Module); i++)
  {
    devices_[i] = devices[i]; 
  }
}

/*
 * Main logic which declare what should do robot in different situations
 */
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

/*
 * Initialize all devices before main loop
 */
void Logic::init()
{
  //Init Seral
  
  Serial.begin(9600);
  Serial.println("[CORE] Initializing");

  for (int i = 0; i < sizeof(devices_) / sizeof(Module); i++)
    ultrasonic.addDevice(devices_[i]);

  //voice.init();
  delay(500);
}

/*
 * Exec functions when recived external commands
 */
void Logic::sendCommand(short unsigned int command)
{
  if ( command == -1 ) stopMoving();
  if ( command == 0 ) goForward();
  if ( command == 1 ) goBackward();
  if ( command == 2 ) moveLeft();
  if ( command == 3 ) moveRight();
}

/*
 * Getting external commands
 */
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
