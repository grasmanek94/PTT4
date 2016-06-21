#include <Servo.h>

Servo servo;
int servoPin = 9;

int filter = 500;
int lower_bound = 185;

enum State
{
  Wait,
  Measure
};

State state;

int servo_rest = 87;
int servo_reject = 175;
int servo_allow = 5;

void setup() {
  Serial.begin(9600);
  servo.attach(servoPin);
  
  servo.write(servo_rest);
  delay(1000);
}

void loop() {
  int v = analogRead(A0);
  int allow = 0;
  
  if (state == Wait &&
      v > lower_bound)
  {
    state = Measure;
    return;
  }
  else if (state == Measure &&
           (v > filter ||
            filter == -1))
  {
    allow = 1;
  }
  else if (state == Measure)
  {
    allow = 2;
  }
  
  if (state == Measure &&
      allow == 1)
  {
    Serial.print("allow ");
    Serial.println(v);
    servo.write(servo_allow);
    delay(1000);
    servo.write(servo_rest);
    state = Wait;
  }
  else if (state == Measure &&
           allow == 2)
  {
    Serial.print("reject ");
    Serial.println(v);
    servo.write(servo_reject);
    delay(1000);
    servo.write(servo_rest);
    state = Wait;
  }

  delay(1500);
}

