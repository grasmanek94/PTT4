#include <MCP2515.h>
#define ArraySize 10
#define ServoPin 3
#define SensorPin A0
#include <Servo.h>
#define CS_PIN    85
#define RESET_PIN  7
#define INT_PIN    84

MCP2515 can;

struct CustomCanMessage {
  byte senderAddress;
  byte data1; //Server:module1
  byte data2; //Server:policy1
  byte data3; //Server:module2
  byte data4; //Server:policy2
  byte data5; //Server:module3
  byte data6; //Server:policy3
  byte empty;
};

CustomCanMessage messageTrue;
CustomCanMessage messageFalse;
int canReceiveTimeoutMs = 20;
int canTransmitTimeoutMs = 20;
int Policy = 1;

int Average = 0;
bool IsInitialised = false;
bool triggering = false;
bool IsAllowedToPass = false;
int AvgArray[ArraySize];
int Index = 0;
Servo myServo;

int small_marble_size = 680;
int large_marble_size = 580;
int Threshold = 6300;
int Default_value = 6400;

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
  messageTrue = CreateMessageTrue();
  messageFalse = CreateMessageFalse();
  delay(200);
}

void loop() {
  if (SetPolicy())
  {
    Serial.println("set Policy");
  }
  //Serial.println(Average);
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
    int CurrentVal = ReadVal();
    CheckSizeOfMarbleToPolicy(CurrentVal);
  }
  delay(1);
  digitalWrite(13, LOW);
}

int CheckSizeOfMarbleToPolicy(int CurrentVal)
{
  if (IsAllowedToPass)
  {
    if (Policy == 1)
    {
      if (state == Measure && CurrentVal > large_marble_size && CurrentVal < small_marble_size)
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
      if (state == Measure && CurrentVal > large_marble_size && CurrentVal > small_marble_size)
      {
        RejectMarble();
      }
    }
    if (Policy == 3)
    {
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
  transmitCAN(messageTrue);
  IsAllowedToPass = false;
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
  transmitCAN(messageFalse);
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


