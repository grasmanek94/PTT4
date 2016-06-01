#define ArraySize 10
#define ServoPin 3
#define SensorPin A0
#include <Servo.h>

int Average = 0;
bool IsInitialised = false;
bool triggering = false;
int AvgArray[ArraySize];
int Index = 0;
Servo myServo;

int small_marble_size = 650;
int large_marble_size = 600;
int Threshold = 6400;
int Default_value = 6600;

enum State
{
  Wait,
  Measure
};

State state;

int servo_rest = 138;
int servo_reject = 180;
int servo_allow = 10;

void setup()
{
  Serial.begin(9600);
  myServo.attach(ServoPin);
  myServo.write(servo_rest);
  delay(200);
}

void loop() {
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
    int CurrentVal = analogRead(SensorPin);
    Serial.println(CurrentVal);
    if (state == Measure && CurrentVal > large_marble_size && CurrentVal < small_marble_size)//(Average > large_marble_size && Average < small_marble_size))
    {
      Serial.println("allow");
      myServo.write(servo_allow);
      delay(1000);
      myServo.write(servo_rest);
      state = Wait;
      delay(500);
    }
    if (state == Measure && CurrentVal < large_marble_size && CurrentVal < small_marble_size)
    {
      Serial.println("reject");
      myServo.write(servo_reject);
      delay(1000);
      myServo.write(servo_rest);
      state = Wait;
      delay(500);
    }
  }
    delay(1);
  }

