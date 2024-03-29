#include <ADJDS311.h>
#include <Wire.h>
#include <Servo.h>
#include "canx.h"

#define doorstand 30
#define beginstand 82
#define wegstand 180
#define GROEN  1
#define WIT    2
#define ANDERS 3
#define ALLES  4
#define NIETS  5

Servo myservo;  // create servo object to control a servo

int sensorLed_pin = 2; //LED on the ADJDS-311
int pos = 0;    // variable to store the servo position
int mem[] = {0, 0, 0};
int newmem[] = {0, 0, 0};
int kleurkeuze = GROEN;

ADJDS311 colorSensor(sensorLed_pin);
CustomCanMessage can_message;
CANMSG msg;

// SDA - 4
// SCL - 5
void setup()
{
  Serial.begin(9600);
  SPI.setClockDivider(SPI_CLOCK_DIV8);

  InitCan();

  pinMode(sensorLed_pin, OUTPUT);
  colorSensor.init();
  colorSensor.ledOn(); //turn LED on

  myservo.attach(3);  // attaches the servo on pin 9 to the servo object
  myservo.write(beginstand);
  delay(2000);
  //Calibrate white
  //Need to hold white card in front (1-3mm) of it to calibrate from
  colorSensor.calibrate();
  Serial.println("remove");
  delay(2000);
  RGBC begincolor = colorSensor.read(); //read the color
  mem[0] = begincolor.red;
  mem[1] = begincolor.green;
  mem[2] = begincolor.blue;
}

void loop()
{
  RGBC color = colorSensor.read(); //read the color

  if (can.receiveCANMessage(&msg, CAN_MS_TIMEOUT) && ParseMessage(msg, can_message))
  {
    if (can_message.senderAddress == CAN_Address_Server)
    {
      CustomCanServerMessage* policy = (CustomCanServerMessage*)&can_message;

      switch (policy->policy_kleur)
      {
        case GROEN:
        case WIT:
        case ANDERS:
        case ALLES:
        case NIETS:
          kleurkeuze = policy->policy_kleur;
          break;
      }
    }
    else
    {
      if (can_message.receiverAddress == CAN_MyAddress)
      {

      }
      else if (can_message.receiverAddress == CAN_Address_Broadcast)
      {
        //received broadcast message
      }
    }
  }
    readRGB(color);
}

void readRGB(RGBC color)
{
  //RGBC is an array of red/green/blue/clear readings
  //Take a RGBC, and try to reproduce it on an RGB LED
  //This does not work very well as is because of how colors/our eyes work
  newmem[0] = map(color.red, 0, 1024, 0, 255);
  Serial.print("R");
  Serial.print(newmem[0]);
  newmem[1] = map(color.green, 0, 1024, 0, 255);
  Serial.print(" G");
  Serial.print(newmem[1]);
  newmem[2] = map(color.blue, 0, 1024, 0, 255);
  Serial.print(" B");
  Serial.println(newmem[2]);
  if ((newmem[0] - mem[0]) > 1 || (newmem[0] - mem[0]) < - 1 ||(newmem[1] - mem[1]) > 1 || (newmem[1] - mem[1]) < - 1 ||(newmem[2] - mem[2]) > 1 || (newmem[2] - mem[2]) < - 1)
  {
    delay(500);
    color = colorSensor.read(); //read the color
    newmem[0] = map(color.red, 0, 1024, 0, 255);
    newmem[1] = map(color.green, 0, 1024, 0, 255);
    newmem[2] = map(color.blue, 0, 1024, 0, 255);
    boolean isdoorgelaten = false;
    switch (kleurkeuze)
    {
      case WIT:
        if (newmem[0] > 200 && newmem[1] > 200 && newmem[2] > 180)
        {
          myservo.write(doorstand);
          delay(750);
          myservo.write(beginstand);
          isdoorgelaten = true;
        }
        else
        {
          myservo.write(wegstand);
          delay(750);
          myservo.write(beginstand);
        }
        break;
      case GROEN:
        if (newmem[0] < 240 && newmem[1] > 150 && newmem[2] < 200)
        {
          myservo.write(doorstand);
          delay(750);
          myservo.write(beginstand);
          isdoorgelaten = true;
        }
        else
        {
          myservo.write(wegstand);
          delay(750);
          myservo.write(beginstand);
        }
        break;
      case ANDERS:
        if (newmem[0] < 160 && newmem[1] < 150 && newmem[2] < 160)
        {
          myservo.write(doorstand);
          delay(750);
          myservo.write(beginstand);
          isdoorgelaten = true;
        }
        else
        {
          myservo.write(wegstand);
          delay(750);
          myservo.write(beginstand);
        }
        break;
      case ALLES:
        myservo.write(doorstand);
        delay(750);
        myservo.write(beginstand);
        isdoorgelaten = true;
        break;
      case NIETS:
        myservo.write(wegstand);
        delay(750);
        myservo.write(beginstand);
        break;
    }

    if (isdoorgelaten)
    {
      transmitCAN(messagePassed);
    }
    else
    {
      Serial.println("rejected");
      transmitCAN(messageRejected);
    }

    delay(400);
    color = colorSensor.read(); //read the color
    newmem[0] = map(color.red, 0, 1024, 0, 255);
    newmem[1] = map(color.green, 0, 1024, 0, 255);
    newmem[2] = map(color.blue, 0, 1024, 0, 255);
  }

  mem[0] = newmem[0];
  mem[1] = newmem[1];
  mem[2] = newmem[2];
  delay(400);
}
