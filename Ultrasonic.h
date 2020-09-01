#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define MIN_DISTANCE 5
#define ULTRASONICDELAY_LOW 3
#define ULTRASONICDELAY_HIGH 15

enum deviceType
{
  FRONT = 0,
  BACK = 1
};

struct Module
{
public:
  deviceType type;
  short unsigned int trigPin;
  short unsigned int echoPin;
  long lastDistance;
  bool active = false;
};

class Ultrasonic
{
public:
  Ultrasonic();
  void blinkDevice(Module device);
  void addDevice(Module device, int position = -1);
  bool initDevice(Module device);
  void calcDistance(Module device);
  float getAverage(deviceType type);
  bool canMove(deviceType type);

private:
  /*
   * 4 modules
   * 3 in front of robot
   * 1 in back of it
   */
  Module modules[3];
};

Ultrasonic::Ultrasonic()
{
  Serial.print("Module count: ");
  Serial.println(sizeof(modules) / sizeof(Module));
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++)
    modules[i] = Module();
}

void Ultrasonic::addDevice(Module device, int position)
{
  initDevice(device);
  if (position != -1 && position <= sizeof(modules) / sizeof(Module))
  {
    device.active = true;
    modules[position] = device;
    return;
  }
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++)
  {
    if (!modules[i].active)
    {
      device.active = true;
      modules[i] = device;
      return;
    }
  }
}

bool Ultrasonic::initDevice(Module device)
{
  pinMode(device.trigPin, OUTPUT);
  pinMode(device.echoPin, INPUT);
  calcDistance(device);
  if (device.lastDistance == 0)
  {
    Serial.print("Can't init device with pins: ");
    Serial.print(device.trigPin);
    Serial.print(", ");
    Serial.println(device.echoPin);
    return false;
  }
  return true;
}

void Ultrasonic::calcDistance(Module device)
{
  if (!device.active)
    return;
  long duration0, duration1, distance;
  blinkDevice(device);
  duration0 = pulseIn(device.echoPin, HIGH);
  blinkDevice(device);
  duration1 = pulseIn(device.echoPin, HIGH);
  device.lastDistance = ((duration0 + duration1) / 2) * 0.034/2;
}

float Ultrasonic::getAverage(deviceType type)
{
  long distance;
  short unsigned int modulesCount;
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++)
  {
    if (modules[i].type == type && modules[i].active)
    {
      distance += modules[i].lastDistance;
      modulesCount++;
    }
  }
  return distance / modulesCount;
}

bool Ultrasonic::canMove(deviceType type)
{
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++)
    calcDistance(modules[i]);
  
  float finalDistance = getAverage(type);
  
  if (finalDistance <= MIN_DISTANCE) 
    return false;
  return true;
}

void Ultrasonic::blinkDevice(Module device)
{
  digitalWrite(device.trigPin, LOW);
  delayMicroseconds(ULTRASONICDELAY_LOW);
  digitalWrite(device.trigPin, HIGH);
  delayMicroseconds(ULTRASONICDELAY_HIGH);
  digitalWrite(device.trigPin, LOW);
}

#endif
