#include <ADJDS311.h>
#include <Wire.h>
#include <SPI.h>
#include <MCP2515.h>
#include <Servo.h>

CANMSG msg;
MCP2515 can;

#define doorstand 30
#define beginstand 80
#define wegstand 180
#define GROEN  1
#define WIT    2
#define DONKER 3
#define ALLES  4

Servo myservo;  // create servo object to control a servo

const int canReceiveTimeoutMs = 20;
int sensorLed_pin = 2; //LED on the ADJDS-311
int pos = 0;    // variable to store the servo position
int mem[] = {0, 0, 0};
int newmem[] = {0, 0, 0};
int kleurkeuze = WIT;
ADJDS311 colorSensor(sensorLed_pin);

// SDA - 4
// SCL - 5

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

void loop() {
  RGBC color = colorSensor.read(); //read the color
  int i = can.receiveCANMessage(&msg, canReceiveTimeoutMs);
  if (i && (msg.data[0] == 1))
  {
    switch (msg.data[1])
    {
      case 1:
        kleurkeuze = GROEN;
        break;
      case 2:
        kleurkeuze = WIT;
        break;
      case 3:
        kleurkeuze = DONKER;
        break;
      case 4:
        kleurkeuze = ALLES;
        break;
    }
    readRGB(color);
    delay(100);
  }
}

void sendCanMessage(boolean doorgelaten)
{
  CANMSG canSendMsgBack;
  canSendMsgBack.adrsValue = 0xDD;
  canSendMsgBack.isExtendedAdrs = false;
  canSendMsgBack.rtr = false;
  canSendMsgBack.dataLength = 8;
  canSendMsgBack.data[0] = 0xDD;
  canSendMsgBack.data[1] = 0xDD;
  canSendMsgBack.data[2] = 0xDD;
  canSendMsgBack.data[3] = 0xDD;
  canSendMsgBack.data[4] = 0xDD;
  canSendMsgBack.data[5] = 0xDD;

  if (doorgelaten)
  {
    canSendMsgBack.data[6] = 0xFF;
  }
  else
  {
    canSendMsgBack.data[6] = 0xAA;
  }
  canSendMsgBack.data[7] = 0xDD;
  can.transmitCANMessage(canSendMsgBack, canReceiveTimeoutMs);
}

void readRGB(RGBC color) {
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

  if ((newmem[0] - mem[0]) > 6 || (newmem[0] - mem[0]) < -6)
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
        if (newmem[0] < 200 && newmem[1] > 150 && newmem[2] < 200)
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
      case DONKER:
        if (newmem[0] < 150 && newmem[1] < 150 && newmem[2] < 150)
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
    }
    sendCanMessage(isdoorgelaten);
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
