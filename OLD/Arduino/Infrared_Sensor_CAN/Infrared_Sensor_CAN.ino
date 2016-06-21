#include <Servo.h>
#include <SPI.h>
#include <MCP2515.h>

#define ArraySize 10
#define ServoPin 3
#define SensorPin A0

/*
 * Defines for CAN addresses
 */
#define CANSERVER 0xAA
#define IRMODULE  0xBB

int Average = 0;
bool IsInitialised = false;
bool triggering = false;
int AvgArray[ArraySize];
int Index = 0;
Servo myServo;

int small_marble_size = 680;
int large_marble_size = 580;
int Threshold = 6300;
int Default_value = 6400;

CANMSG msgReceive;
MCP2515 can;



enum State
{
  Wait,
  Measure
};

State state;

int servo_rest = 122;
int servo_reject = 180;
int servo_allow = 10;

void setup()
{
  Serial.begin(9600);
  myServo.attach(ServoPin);
  myServo.write(servo_rest);
  Blink();
  delay(200);
  
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  
  if(can.initCAN(CAN_BAUD_100K) == 0)
  {
    Serial.println("initCAN() failed");
    while(1);
  }
  
  if(can.setCANNormalMode(LOW) == 0) //normal mode non single shot
  {
    Serial.println("setCANNormalMode() failed");
    while(1);
  }
}

void loop() {
  Serial.println(Average);
  int allow = 0;
  StateMachine(&allow);
  if (allow == 0)
  {
    MovingAverage();
  }
  handleCAN(can.receiveCANMessage(&msgReceive, 100));
  if (allow) {
    int msgLength = 2;
    byte msgData[2] = {0xBE, 0xEF};
    sendCanMsg(IRMODULE, msgLength, msgData);
  }
}

void MovingAverage()
{
  if (!IsInitialised)
  {
    Initialise();
  }
  CalculateAverage();
  AddSingleValueToArray();
}

void Initialise()
{
  int i = 0;
  for (; i < ArraySize; i++)
  {
    AvgArray[i] = analogRead(SensorPin);
  }
  CalculateAverage();
  Default_value = Average;
  IsInitialised = true;
}

void CalculateAverage()
{
  int avg = 0;
  int j = 0;
  for (; j < ArraySize; j++)
  {
    avg += AvgArray[j];
  }
  avg / ArraySize;
  Average = avg;
}

void AddSingleValueToArray()
{
  AvgArray[Index] = analogRead(SensorPin);
  Index++;
  if (Index >= ArraySize)
  {
    Index = 0;
  }
}

void StateMachine(int* allow)
{
  if (state == Wait && Average > Default_value)
  {
    //Serial.println("Measure");
    Serial.print("Thing detected");
    state = Measure;
    *allow = 0;
    delay(500);
    return;
  }
  if (state == Measure && Average < Threshold)
  {
    delay(200);
    int CurrentVal = analogRead(SensorPin);
    Serial.println(CurrentVal);
    digitalWrite(13, HIGH);
    if (state == Measure && CurrentVal > large_marble_size && CurrentVal < small_marble_size)//(Average > large_marble_size && Average < small_marble_size))
    {
      Serial.println("allow");
      myServo.write(servo_allow);
      delay(1000);
      myServo.write(servo_rest);
      state = Wait;
      delay(500);
      digitalWrite(13, LOW);
    }
    if (state == Measure && CurrentVal < large_marble_size && CurrentVal < small_marble_size)
    {
      Serial.println("reject");
      myServo.write(servo_reject);
      delay(1000);
      myServo.write(servo_rest);
      state = Wait;
      delay(500);
      digitalWrite(13, LOW);
    }
  }
  delay(1);
  digitalWrite(13, LOW);
}

void Blink()
{
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
}

void handleCAN(bool messageReceived) {
  //msgReceive ipv msg
  //int i = can.receiveCANMessage(&msg, 100);

  msgReceive.dataLength;
  msgReceive.adrsValue;

  // We received a thing from the server, this is probably
  if (msgReceive.adrsValue == CANSERVER) {
    // Do something with msgReceive.data[]
  }
  
  /*
  int data = msg.data[2];
  if (msg.adrsValue == 0xFF) {
    digitalWrite(led2, HIGH);
    Serial.print(">0xFF: ");
    Serial.print(data);
    Serial.print(";\n");
  }
  else {
    digitalWrite(led2, LOW);
  }
  if (msg.adrsValue == 0xDD) {
    digitalWrite(led3, HIGH);
    Serial.print(">0xDD: ");
    Serial.print(data);
    Serial.print(";\n");
  }
  else {
    digitalWrite(led3, LOW);
  }
  if(msg.data[2] == 123) {
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    delay(100);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    delay(100);
    Serial.println("data received");
  }
  */
}

void sendCanMsg(unsigned long adrsValue, int dataLength, byte *data) {
  CANMSG msg;
  msg.adrsValue = adrsValue;
  msg.isExtendedAdrs = false;
  msg.rtr = false;
  msg.dataLength = dataLength;
  for (int i = 0; i < dataLength; i++) {
    msg.data[i] = data[i];
  }
  can.transmitCANMessage(msg, 1000);
  /*msg.adrsValue = 0x7df;
  msg.isExtendedAdrs = false;
  msg.rtr = false;
  msg.dataLength = 8;
  msg.data[0] = 0x02;
  msg.data[1] = 0x01;
  msg.data[2] = 123;
  msg.data[3] = 0;
  msg.data[4] = 0;
  msg.data[5] = 0;
  msg.data[6] = 0;
  msg.data[7] = 0;
  can.transmitCANMessage(msg, 1000);
  Serial.println("data transmitted");*/
}

