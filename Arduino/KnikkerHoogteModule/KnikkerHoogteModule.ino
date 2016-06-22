#include "canx.h"
#define ArraySize 10
#define ServoPin 3
#define SensorPin A0
#include <Servo.h>
#define CS_PIN    85
#define RESET_PIN  7
#define INT_PIN    84
#define ErrorFactor 10
#define SmallMarbleError 20
#define LargeMarbleError 40

/*
 * PolicyTable:
 * 1.Pass Through Big, Reject Small
 * 2.Pass Through Small, Reject Big
 * 3.Pass Through All 
 * 4.Reject All
 */

int Policy = 3;

int Average = 0;
bool IsInitialised = false;
bool triggering = false;
bool IsAllowedToPass = true;//set back to false when CAN
int AvgArray[ArraySize];
int Index = 0;
Servo myServo;

int small_marble_size = 422;
int large_marble_size = 440;
int Threshold;
int Default_value = 0;

enum State
{
  Wait,
  Measure
};

State state;

int servo_rest = 124;
int servo_reject = 180;
int servo_allow = 10;

void setup()
{
  Serial.begin(9600);
  myServo.attach(ServoPin);
  myServo.write(servo_rest);
  InitCan();
  Blink();
  Initialise();
  Threshold = Default_value + ErrorFactor;
  small_marble_size = Threshold + SmallMarbleError;
  large_marble_size = Threshold + LargeMarbleError;
  Serial.print("Threshold:");
  Serial.println(Threshold);
  Serial.print("small_marble_size:");
  Serial.println(small_marble_size);
  Serial.print("large_marble_size:");
  Serial.println(large_marble_size);
  delay(200);
}

void PrintMessage(CustomCanMessage& message)
{
  Serial.print(message.receiverAddress, HEX);
  Serial.print(',');
  Serial.print(message.senderAddress, HEX);
  Serial.print(',');
  Serial.print(message.function, HEX);
  Serial.print(',');
  Serial.print(message.measure, HEX);
  Serial.print(',');
  Serial.print(message.diagnostics, HEX);
  Serial.print(',');
  Serial.print(message.empty1, HEX);
  Serial.print(',');
  Serial.print(message.empty2, HEX);
  Serial.print(',');
  Serial.print(message.empty3, HEX);
  Serial.print("");
}

bool SetPolicy()
{
  CANMSG canReceived;
  CustomCanMessage can_message;
  if (can.receiveCANMessage(&canReceived, CAN_MS_TIMEOUT) && ParseMessage(canReceived, can_message))
  {
    if (can_message.senderAddress == CAN_Address_Server)
    {
      CustomCanServerMessage* policy = (CustomCanServerMessage*)&can_message;

      PrintMessage(can_message);

      Policy = policy->policy_hoogte;
      return true;
    }
    else
    {
      if (can_message.receiverAddress == CAN_MyAddress)
      {
        //received message addresed to me
      }
      else if (can_message.receiverAddress == CAN_Address_Broadcast)
      {
        //received broadcast message
        if (can_message.senderAddress == CAN_Address_Color && (can_message.function == BROADCAST_MARBLE_ACCEPTED || can_message.function == BROADCAST_MARBLE_ACCEPTED))
        {
          IsAllowedToPass = true;
        }
      }
    }
  }

  return false;
}

void loop()
{
  if (SetPolicy())
  {
    Serial.println("set Policy");
  }

  Serial.println(Average);
  int allow = 0;
  StateMachine(&allow);
  if (allow == 0)
  {
    MovingAverage();
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
  avg = avg / ArraySize;
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
  if (state == Wait && Average < Default_value)
  {
    //Serial.println("Measure");
    Serial.print("Thing detected");
    state = Measure;
    *allow = 0;
    delay(500);
    return;
  }
  if (state == Measure && Average > Threshold)
  {
    Serial.println("Trigger");
    Serial.println(Policy);
    int CurrentVal = ReadVal();
    CheckSizeOfMarbleToPolicy(CurrentVal);
  }
  delay(1);
  digitalWrite(13, LOW);
}

void CheckSizeOfMarbleToPolicy(int CurrentVal)
{
  if (IsAllowedToPass)
  {
    if (Policy == 1)
    {
      if (state == Measure && CurrentVal > large_marble_size && CurrentVal > small_marble_size)
      {
        AllowMarble();
      }
      if (state == Measure && CurrentVal < large_marble_size && CurrentVal < small_marble_size)
      {
        RejectMarble();
      }
    }
    if (Policy == 2)
    {
      if (state == Measure && CurrentVal < large_marble_size && CurrentVal > small_marble_size)
      {
        AllowMarble();
      }
      if (state == Measure && CurrentVal < large_marble_size && CurrentVal < small_marble_size)
      {
        RejectMarble();
      }
    }
    if (Policy == 3)
    {
      Serial.println("ping");
      AllowMarble();
    }
    if (Policy == 4)
    {
      RejectMarble();
    }
  }
}

int ReadVal()
{
  delay(200);
  int CurrentVal = analogRead(SensorPin);
  Serial.println(CurrentVal);
  digitalWrite(13, HIGH);
  return CurrentVal;
}

void AllowMarble()
{
  Serial.println("allow");
  myServo.write(servo_allow);
  delay(1000);
  myServo.write(servo_rest);
  transmitCAN(messagePassed);
  //IsAllowedToPass = false;
  state = Wait;
  delay(500);
  digitalWrite(13, LOW);
}

void RejectMarble()
{
  Serial.println("reject");
  myServo.write(servo_reject);
  delay(1000);
  myServo.write(servo_rest);
  transmitCAN(messageRejected);
  state = Wait;
  delay(500);
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

