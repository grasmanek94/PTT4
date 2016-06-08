#include "motorshield.h"

DualVNH5019MotorShield motorController;

void setup() 
{
  motorController.init();
  pinMode(7, INPUT_PULLUP);
  motorController.setSpeeds(400,400);
}

void loop() 
{

}

