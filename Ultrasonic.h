#ifndef ULTRASONIC_H
#define ULTRASONIC_H

// Switch to true for view debug messages
#define DEBUGMODE false

// There are limits for modules
#define MIN_DISTANCE 70
#define ULTRASONICDELAY_LOW 3
#define ULTRASONICDELAY_HIGH 15

/*
 * There are few device type (you can add more)
 * NONE - uninitialized empty device
 * FRONT There are devices in front of robot
 * BACK Same but in back
 */
enum deviceType
{
  NONE = -1,
  FRONT = 0,
  BACK = 1
};

/*
 * There are ultrasonic module structure which contains
 * Type of current device
 * Trig pin
 * Echo pin
 * And last scaned distance
 */
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
   * Current robot device list:
   * 4 modules
   * 3 in front of robot
   * 1 in back of it
   */
  Module modules[3];
};

/*
 * Init variables
 */
Ultrasonic::Ultrasonic()
{
  Serial.print("Module count: ");
  Serial.println(sizeof(modules) / sizeof(Module));
  struct Module initModule {.type = NONE};
  for (int i = 0; i < sizeof(modules) / sizeof(Module); i++)
    modules[i] = initModule;
}

/*
 * Adding new device to list
 * Also you can set new position by force if you will set position
 */
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

/*
 * Initialize new device
 */
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
    Serial.print(".Device recived data: ");
    Serial.println(device.lastDistance);
    return false;
  }
  return true;
}

float Ultrasonic::calcDistance(Module device)
{
  if (device.type == NONE)
    return 0;
  long duration;
  blinkDevice(device);
  duration = pulseIn(device.echoPin, HIGH);
  // converting to centimeteres
  return duration / 29 / 2;
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

/*
 * If at least one module will recive value which will be less than declared minimum,
 * function will return false
 */
bool Ultrasonic::canMove(deviceType type)
{
  for (int i = 0; i < 4; i++)
    modules[i].lastDistance = calcDistance(modules[i]);

  for (int i = 0; i < 4; i++)
  {
    
    Serial.print( i );
    Serial.print( " returned: " );
    Serial.print( modules[i].lastDistance );
    if (modules[i].lastDistance <= MIN_DISTANCE)
    { 
      Serial.println();
      return false;
    }      
  }
  Serial.println();
  
  return true;
}

/*
 * There are function blinking trig pin of ultrasonic 
 */
void Ultrasonic::blinkDevice(Module device)
{
  digitalWrite(device.trigPin, LOW);
  delayMicroseconds(ULTRASONICDELAY_LOW);
  digitalWrite(device.trigPin, HIGH);
  delayMicroseconds(ULTRASONICDELAY_HIGH);
  digitalWrite(device.trigPin, LOW);
}

#endif
