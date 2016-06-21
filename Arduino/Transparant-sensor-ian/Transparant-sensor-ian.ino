#include <SPI.h>
#include <MCP2515.h>
#include <Servo.h>
#include "canx.h"

CANMSG msg;
CustomCanMessage message;

Servo servo;
int servoPin = 3;
int servo_rest = 60;
int servo_reject = 165;
int servo_allow = 10;

int filter = 550;
int oldstate = 300;
int lower_bound = 185;
int threshold = 20;
int value;

const int AverageSize = 5;
int MovingAverage[AverageSize] = {oldstate,oldstate,oldstate,oldstate,oldstate};

enum State
{
  Wait,
  Measure
};

enum Mode
{
  Transparent,
  Opaque,
  Both,
  None
};

enum Allow
{
  Yes,
  No
};

State state = Wait;
Mode mode = Both;
Allow allow = No;


void setup()
{
  Serial.begin(9600);
  servo.attach(servoPin);
  servo.write(servo_rest);
  InitCan();
  delay(1000);
  oldstate = CalculateAverage();
}

void loop()
{
  ReadMessage();
  MeasureMarble();
  SendData(); 
}

void ReadMessage()
{
  int i = can.receiveCANMessage(&msg, 20);
  if (i && (msg.data[0] == 7) && (msg.data[6] == 3))
  {
    switch (msg.data[7])
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
      case 4:
        mode = None;
        break;
    }
    Serial.println("data received");
  }
}

void MeasureMarble()
{
  value = CalculateAverage();
  if (state == Wait && (value < oldstate - threshold || value > oldstate + threshold))
  {
    state = Measure;
  }
  if (state == Measure && value > filter)
  {
    switch (mode)
    {
      case Transparent:
        allow = No;
        break;
      case Opaque:
        allow = Yes;
        break;
      case Both:
        allow = Yes;
        break;
      case None:
        allow = No;
        break;
    }
  }
  else
  {
    switch (mode)
    {
      case Transparent:
        allow = Yes;
        break;
      case Opaque:
        allow = No;
        break;
      case Both:
        allow = Yes;
        break;
      case None:
        allow = No;
        break;
    }
  }
}

void SendData()
{
  if(state == Measure && allow == Yes)
  {
    Serial.print("allow ");
    Serial.println(value);
    message = messagePassed;
    transmitCAN(message);
    servo.write(servo_allow);
    delay(1000);
    servo.write(servo_rest);
    delay(1000);
    oldstate = CalculateAverage();
    delay(500);
    state = Wait;
  }
  else if (state == Measure &&
           allow == No)
  {
    Serial.print("reject ");
    Serial.println(value);
    message = messageRejected;
    transmitCAN(message);
    servo.write(servo_reject);
    delay(1000);
    servo.write(servo_rest);
    delay(1000);
    oldstate = CalculateAverage();
    delay(500);
    state = Wait;
  }
  else
  {
    oldstate = CalculateAverage();
  }
}

int CalculateAverage()
{
  int temp = 0;
  for(int i = 0; i < 10; i++)
  {
    temp += analogRead(A0);
    delay(50);
  }
  temp = temp / 10;
  return temp;
}
