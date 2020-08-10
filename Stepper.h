#ifndef MYSTEPPER_H
#define MYSTEPPER_H

/*     
 *     Controll
 *     
 *     1  2 3 4
 *  1  -  # + #
 *  2  +  # - #
 *  3  #  - # +
 *  4  #  + # -
 *  
 */

class MyStepper 
{
public:
  MyStepper::MyStepper(
    int pinA,
    int pinB, 
    int pinC, 
    int pinD,
    int mnStep,
    int mxStep) 
    : maxStep(mxStep),
      minStep(mnStep)
  {
    pins[0] = pinA;
    pins[1] = pinB;
    pins[2] = pinC;
    pins[3] = pinD;
  }

  void step1() 
  {
    digitalWrite(pins[0], LOW);
    digitalWrite(pins[1], HIGH);
    digitalWrite(pins[2], HIGH);
    digitalWrite(pins[3], HIGH);
  }

  void step2()
  {
    digitalWrite(pins[0], LOW);
    digitalWrite(pins[1], LOW);
    digitalWrite(pins[2], HIGH);
    digitalWrite(pins[3], HIGH);
  }

  void step3()
  {
    digitalWrite(pins[0], HIGH);
    digitalWrite(pins[1], LOW);
    digitalWrite(pins[2], HIGH);
    digitalWrite(pins[3], HIGH);
  }

  void step4()
  {
    digitalWrite(pins[0], HIGH);
    digitalWrite(pins[1], LOW);
    digitalWrite(pins[2], LOW);
    digitalWrite(pins[3], HIGH);
  }

  void step5()
  {
    digitalWrite(pins[0], HIGH);
    digitalWrite(pins[1], HIGH);
    digitalWrite(pins[2], LOW);
    digitalWrite(pins[3], HIGH);
  }

  void step6()
  {
    digitalWrite(pins[0], HIGH);
    digitalWrite(pins[1], HIGH);
    digitalWrite(pins[2], LOW);
    digitalWrite(pins[3], LOW);
  }

  void step7()
  {
    digitalWrite(pins[0], HIGH);
    digitalWrite(pins[1], HIGH);
    digitalWrite(pins[2], HIGH);
    digitalWrite(pins[3], LOW);
  }

  void step8()
  {
    digitalWrite(pins[0], LOW);
    digitalWrite(pins[1], HIGH);
    digitalWrite(pins[2], HIGH);
    digitalWrite(pins[3], LOW);
  }

  void checkMade() 
  {
    int counter = 0;
    while (counter != 20)
    {
      step1();
      delay(2);
      step2();
      delay(2);
      step3();
      delay(2);
      step4();
      delay(2);
      step5();
      delay(2);
      step6();
      delay(2);
      step7();
      delay(2);
      step8();
      delay(2);
      counter += 1;
    }
    counter = 0;
  }
  
  void goStep(int count)
  {
    for (int i = 0; i < count; i++) {
      
    }
  }

  void msetSpeed(int uspeed) {
    if (uspeed == mspeed)
      return
    mspeed = uspeed;
  }
  
private:
  int pins[3];
  int maxStep;
  int minStep;
  int mspeed;
};

#endif
