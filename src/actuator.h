#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

// Actuator
float currentActuatorExtension=50;
int actuator_pin = 4;       //actuator Pin
Servo actuator;             //create servo objects to control the linear actuator

void SetStrokePerc(float strokePercentage)
{
  if ( strokePercentage >= 1.0 && strokePercentage <= 99.0 )
  {
    int usec = 1000 + strokePercentage * ( 2000 - 1000 ) / 100.0 ;
    actuator.writeMicroseconds( usec );
  }
  currentActuatorExtension=strokePercentage;
}