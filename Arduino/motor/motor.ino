#include "motorshield.h"

DualVNH5019MotorShield motorController;

void setup() 
{
  pinMode(7, INPUT_PULLUP);
  motorController.setSpeeds(400,400);
}

void loop() 
{

}

