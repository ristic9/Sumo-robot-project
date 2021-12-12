#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
 
#define LED  13
 
#define QTR_THRESHOLD 1500 // microseconds
#define MAX_SPEED    400
#define DURATION 200 // ms

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
 
#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];
 
ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

void waitForButtonAndCountDown()
{
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);
}
 
void setup()
{ 
  pinMode(LED, HIGH);
  waitForButtonAndCountDown();
}

unsigned long pre = 0;
void loop()
{
  if (button.isPressed())
  {
    motors.setSpeeds(0, 0);
    button.waitForRelease();
  }
  randomSeed(millis());
  int random1 = random(20,50);
  #define RAN_DURATION DURATION + random1
  sensors.read(sensor_values);
  unsigned long current = millis();
  
  if ((sensor_values[0] < QTR_THRESHOLD) && (sensor_values[5] < QTR_THRESHOLD)) // makes it go backwards and turn if the leftmost and rightmost sensors detect a line
  {
    motors.setSpeeds(-MAX_SPEED, -MAX_SPEED);
    delay(RAN_DURATION);
    motors.setSpeeds(MAX_SPEED, -MAX_SPEED);
    delay(RAN_DURATION);
    go_forward();
  }
  else if (sensor_values[0] < QTR_THRESHOLD) // if leftmost sensor detects line, turn to the right 
  {
    turn_right();
    pre = current;
  }
  else if (sensor_values[5] < QTR_THRESHOLD)// if rightmost sensor detects line, turn to the left
  {
    turn_left();
    pre=current;
  }
  else
  {
    if (current - pre > 2500)
    {
      motors.setSpeeds(-100, -100);
      motors.setSpeeds(MAX_SPEED, MAX_SPEED);
    }
    // if it doesn't detect anything it goes full speed straight
    go_forward();
  }
}

void turn_right()
{
  randomSeed(millis());
  
  int random1 = random(100,300);
  int rspeed1 = random(200,400);
  motors.setSpeeds(-rspeed1, -rspeed1);
  delay(random1);
  motors.setSpeeds(-rspeed1, rspeed1);
  delay(random1);
  go_forward();
}

void turn_left()
{
  randomSeed(millis());
  int random1 = random(100,300);
  
  int rspeed2 = random(200,400);
  motors.setSpeeds(-rspeed2, -rspeed2);
  delay(random1);
  motors.setSpeeds(rspeed2, -rspeed2);
  delay(random1);
  go_forward();
}

void go_forward()
{
  motors.setSpeeds(MAX_SPEED, MAX_SPEED);
}
