
#include <SPI.h>
#include <MCP2515.h>
#include <Servo.h>

CANMSG msg;
MCP2515 can;

struct SerialMessage {
  byte senderAddress;
  byte module1;
  byte policy1;
  byte module2;
  byte policy2;
  byte module3;
  byte policy3;
  byte empty;
};

SerialMessage message;

Servo servo;
int servoPin = 3;

int filter = 550;
int oldstate = 300;
int lower_bound = 185;

int v;
int allow;

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
  servo.attach(servoPin);
  servo.write(servo_rest);
  InitCan();
  delay(1000);
  oldstate = analogRead(A0);
}

void loop() {
  v = analogRead(A0);
  allow = 0;
  ReadMessage();
  MeasureMarble();
  ExecuteMeasurement();
}

void InitCan()
{
  if (can.initCAN(CAN_BAUD_100K) == 0) {
    Serial.println("initCAN() failed");
    while (1);
  }
  if (can.setCANNormalMode(LOW) == 0) { //normal mode non single shot
    Serial.println("setCANNormalMode() failed");
    while (1);
  }
}

void ReadMessage()
{
  int i = can.receiveCANMessage(&msg, 20);
  if (i && (msg.data[4] = 3))
  {
    switch (msg.data[5])
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
}

void MeasureMarble()
{
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
}

void ExecuteMeasurement()
{
  if (state == Measure &&
      allow == 1)
  {
    Serial.print("allow ");
    Serial.println(v);
    message = CreateMessageTrue();
    transmitCAN(message);
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
    message = CreateMessageFalse();
    transmitCAN(message);
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

SerialMessage CreateMessageTrue()
{
  SerialMessage msg;
  msg.senderAddress = 0XFF;
  msg.module1 = 0xFF;
  msg.policy1 = 0xFF;
  msg.module2 = 0xFF;
  msg.policy2 = 0xFF;
  msg.module3 = 0xFF;
  msg.policy3 = 0xFF;
  msg.empty = 0x00;
  return msg;
}

SerialMessage CreateMessageFalse()
{
  SerialMessage msg;
  msg.senderAddress = 0XAA;
  msg.module1 = 0xAA;
  msg.policy1 = 0xAA;
  msg.module2 = 0xAA;
  msg.policy2 = 0xAA;
  msg.module3 = 0xAA;
  msg.policy3 = 0xAA;
  msg.empty = 0x00;
  return msg;
}

bool transmitCAN(SerialMessage message) {

  CANMSG canmsg;
  canmsg.adrsValue = CAN_MyAddress;
  canmsg.isExtendedAdrs = false;
  canmsg.rtr = false;
  canmsg.dataLength = 8;
  canmsg.data[0] = message.module1;
  canmsg.data[1] = message.policy1;
  canmsg.data[2] = message.module2;
  canmsg.data[3] = message.policy2;
  canmsg.data[4] = message.module3;
  canmsg.data[5] = message.policy3;
  canmsg.data[6] = message.empty;
  canmsg.data[7] = message.empty;
  can.transmitCANMessage(canmsg, canTransmitTimeoutMs);
}

