#ifndef SERVOLIB_H
#define SERVOLIB_H

#include <Servo.h>

class ServoLib 
{
public:

  ServoLib::ServoLib
  (
    int servo0,
    int servo1
  )
    : servopin0(servo0),
      servopin1(servo1)
  {
    control0.attach(servopin0);
    control1.attach(servopin1);

    for (pos0 = 0; pos0 <= 180; pos0 += 1) {
      pos1 = pos0;
      control0.write(pos0);
      control0.write(pos1);             
      delay(15);                  
    }
    
    for (pos0 = 180; pos0 >= 0; pos0 -= 1) {
      pos1 = pos0;
      control0.write(pos0);
      control1.write(pos1);              
      delay(15);
    }
    
  }

  void goFoward() 
  {
    for (pos0 = pos0; pos0 <= 180; pos0++)
    {
      pos1 = -pos0;
      control0.write(pos0);
      control1.write(pos1);
      delay(15);  
    }
  }

  void goBackward()
  {
    for (pos0 = pos0; pos0 >= 0; pos0--)
    {
      pos1 = -pos0;
      control0.write(pos0);
      control1.write(pos1);
      delay(15);
    }
  }
  
private:

  int pos0;
  int pos1;

  int servopin0;
  int servopin1;
  Servo control0;
  Servo control1;
  
};

#endif SERVOLIB_H
