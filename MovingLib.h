#ifndef MOVING_H
#define MOVING_H

class Moving 
{
public:

  Moving::Moving
  (
    int pin1,
    int pin2,
    int pin3,
    int pin4
  )
    : relePin1(pin1),
      relePin2(pin2),
      relePin3(pin3),
      relePin4(pin4)
  {
    pinMode ( relePin1, OUTPUT );
    pinMode ( relePin2, OUTPUT );
    pinMode ( relePin3, OUTPUT );
    pinMode ( relePin4, OUTPUT );    
    delay(100);
  }

  void restore()
  {
    digitalWrite( relePin1, LOW );
    digitalWrite( relePin2, LOW );
    digitalWrite( relePin3, LOW );
    digitalWrite( relePin4, LOW );
  }

  void goFoward() 
  {
    restore();
    delay(20);
    digitalWrite( relePin1, HIGH );
  }

  void goBackward()
  {
    restore();
    delay(20);
    digitalWrite( relePin2, HIGH );
  }

  void turnLeft()
  {
    restore();
    delay(20);
    digitalWrite( relePin3, HIGH );
  }

  void turnRight()
  {
    restore();
    delay(20);
    digitalWrite( relePin4, HIGH );
  }
  
private:

  int relePin1;
  int relePin2;
  int relePin3;
  int relePin4;

  bool moving;
  bool turning;
  
};

#endif MOVING_H
