
#include <SPI.h>
#include <MCP2515.h>
#include <Servo.h>

CANMSG msg;
MCP2515 can;

Servo servo;
int servoPin = 3;

int filter = 550;
int oldstate = 300;
int lower_bound = 185;

enum State
{
  Wait,
  Measure
};

enum Mode
{
  Transparent,
  Opaque,
  Both
};

State state = Wait;
Mode mode = Opaque;

int servo_rest = 60;
int servo_reject = 165;
int servo_allow = 10;

void setup() {
  Serial.begin(9600);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  if (can.initCAN(CAN_BAUD_100K) == 0)
  {
    Serial.println("in itCAN() failed");
    while (1);
  }

  if (can.setCANNormalMode(LOW) == 0) //normal mode non single shot
  {
    Serial.println("setCANNormalMode() failed");
    while (1);
  }
  servo.attach(servoPin);
  servo.write(servo_rest);
  delay(1000);
  oldstate = analogRead(A0);
}

void loop() {
  int v = analogRead(A0);
  int allow = 0;
  int i = can.receiveCANMessage(&msg, 1000);
  if (i && (msg.adrsValue == 123))
  {
    switch (msg.data[2])
    {
      case 1:
        mode = Transparent;
        break;
      case 2:
        mode = Opaque;
        break;
      case 3:
        mode = Both;
        break;
    }
    Serial.println("data received");
  }
  if (state == Wait && (v < oldstate - 8 || v > oldstate + 8))
  {
    state = Measure;
    Serial.println("ik meet iets");
  }
  if (state == Measure && v > filter)
  {
    switch (mode)
    {
      case Transparent:
        allow = 2;
        break;
      case Opaque:
        allow = 1;
        break;
      case Both:
        allow = 1;
        break;
    }
  }
  else
  {
    switch (mode)
    {
      case Transparent:
        allow = 1;
        break;
      case Opaque:
        allow = 2;
        break;
      case Both:
        allow = 1;
        break;
    }
  }

  if (state == Measure &&
      allow == 1)
  {
    Serial.print("allow ");
    Serial.println(v);
    servo.write(servo_allow);
    delay(1000);
    servo.write(servo_rest);
    delay(1000);
    oldstate = analogRead(A0);
    delay(500);
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
    delay(1000);
    oldstate = analogRead(A0);
    delay(500);
    state = Wait;
  }
  else
  {
    oldstate = v;
  }
}

