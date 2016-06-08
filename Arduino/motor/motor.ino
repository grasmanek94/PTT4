#include "motorshield.h"

#define INT_PIN 3

DualVNH5019MotorShield motorController;

volatile long prevTime;
volatile long currTime;

long lastTime;

void setup() 
{
  pinMode(INT_PIN, INPUT_PULLUP);
  motorController.init();
  motorController.setSpeeds(400,400);

  attachInterrupt(digitalPinToInterrupt(INT_PIN), isr, CHANGE);

  Serial.begin(9600);

  lastTime = millis();
}

void loop() 
{
  if ((millis() - lastTime) > 500)
  {
      int delta = currTime - prevTime;
      Serial.println(delta);
      lastTime = millis();
  }
}

void isr()
{
  prevTime = currTime;
  currTime = micros();
}

