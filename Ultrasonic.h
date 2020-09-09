#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#define DEBUGMODE false
#define MIN_DISTANCE 50
#define ULTRASONICDELAY_LOW 3
#define ULTRASONICDELAY_HIGH 15

enum deviceType
{
  NONE = -1,
  FRONT = 0,
  BACK = 1
};

struct Module
{
  deviceType type;
  short unsigned int trigPin;
  short unsigned int echoPin;
  long lastDistance;
};

class Ultrasonic
{
public:
  Ultrasonic();
  void blinkDevice(Module device);
  void addDevice(Module device, int position = -1);
  bool initDevice(Module device);
  float calcDistance(Module device);
  float getAverage(deviceType type);
  bool canMove(deviceType type = FRONT);

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
  struct Module initModule {.type = NONE};
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++)
    modules[i] = initModule;
}

void Ultrasonic::addDevice(Module device, int position)
{
  initDevice(device);
  if (position != -1 && position <= sizeof(modules) / sizeof(Module))
  {
    modules[position] = device;
    return;
  }
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++)
  {
    if (modules[i].type == NONE)
    {
      modules[i] = device;
      return;
    }
  }
}

bool Ultrasonic::initDevice(Module device)
{
  pinMode(device.trigPin, OUTPUT);
  pinMode(device.echoPin, INPUT);
  device.lastDistance = calcDistance(device);
  if (device.lastDistance == 0)
  {
    Serial.print("Can't init device with pins: ");
    Serial.print(device.trigPin);
    Serial.print(", ");
    Serial.print(device.echoPin);
    Serial.print(".Incoming value: ");
    Serial.println(device.lastDistance);
    return false;
  }
  return true;
}

float Ultrasonic::calcDistance(Module device)
{
  if (device.type == NONE)
    return 0;
  long duration0, duration1;
  blinkDevice(device);
  duration0 = pulseIn(device.echoPin, HIGH);
  blinkDevice(device);
  duration1 = pulseIn(device.echoPin, HIGH);
  return ((duration0 + duration1) / 2) * 0.034/2;
}

float Ultrasonic::getAverage(deviceType type)
{
  long distance = 0;
  short unsigned int modulesCount = 0;
  
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++)
  {
    if (modules[i].type == type && modules[i].type != NONE)
    {
      if (DEBUGMODE)
      {
        Serial.print("Device at: ");
        Serial.print(i);
        Serial.print(" Return: ");
        Serial.print(modules[i].lastDistance);
        Serial.println(";");
      }
      distance += modules[i].lastDistance;
      modulesCount++;
    }
  }
  
  if (DEBUGMODE)
  {
    Serial.print("Total distance");
    Serial.print(distance / modulesCount);
    Serial.println();
  } 
  return distance / modulesCount;
}

bool Ultrasonic::canMove(deviceType type)
{
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++)
    modules[i].lastDistance = calcDistance(modules[i]);
  
  float finalDistance = getAverage(type);
  Serial.println(finalDistance);
  
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
