/*
 * Alternative Height sensor code
 * Value @ SensorPin increases as distance to sensor decreases
 * We probably only want to react to values > default
 */


#include "canx.h"

#define ServoPin 3
#include <Servo.h>

#define SensorPin A0
#define CS_PIN    85
#define RESET_PIN 7
#define INT_PIN   84
#define SAMPLES   10    // Used for initial calibration window
                        // and for moving average window

enum MachineState {
  sMeasuring,
  sAction
};

enum MarbleSize {
  Big,
  Small,
  Unknown
};

MachineState state = sMeasuring;

unsigned long previousMillis = 0;
int delayMs = 200;

Servo myServo;
int servo_rest = 124;
int servo_reject = 180;
int servo_allow = 10;

int nothing = 0;
int hysteresis = 20;
int calibrateMs = 2000;

int rawArray[SAMPLES] = {};
int averageIndex = 0;

int marbleSmall = 400;  // need sample
int marbleBig = 500;    // need sample

int Policy = 1; // magic numbers :x

void setup() {
  Serial.begin(9600);
  myServo.attach(ServoPin);
  myServo.write(servo_rest);
  //InitCan();
  
  Serial.println("Calibrating...");
  nothing = calibrate(calibrateMs);
  Serial.print("Nothing: ");
  Serial.println(nothing);
}

void loop() {
  //if (SetPolicy()) {
  //    Serial.println("set Policy");
  //}
  unsigned long currentMillis = millis();
  
  // Update raw average array and move index
  // Index loops back to 0.
  rawArray[averageIndex] = analogRead(SensorPin);
  averageIndex = (averageIndex + 1) % (SAMPLES - 1);
  int currAverage = getAverage(rawArray);

  /*
   * While idling, check if something is detected. Once it's
   * detected, wait delayMs milliseconds while gathering more
   * values and then switch state.
   */
  if (state == sMeasuring) {
    Serial.println(currAverage);
    if (currAverage > (nothing + hysteresis)) {
      if (currentMillis - previousMillis >= delayMs) {
        previousMillis = 0;
        Serial.println("Spotted something!");
        state = sAction;
      }
    }
  }

  /*
   * Once we have a neat value, use this to compare against
   * stuff.
   */
  if (state == sAction) {
    MarbleSize marbleSize = Unknown;
    if (currAverage > marbleBig) {
      Serial.println("big marble");
      marbleSize = Big;
    }
    else if (currAverage > marbleSmall) {
      Serial.println("small marble");
      marbleSize = Small;
    }
    else {
      Serial.println("wtf");
    }
    if (Policy == 1) {
      if (marbleSize == Small) {
        AllowMarble();
      } else {
        RejectMarble();
      }
    }
    if (Policy == 2) {
      if (marbleSize == Big) {
        AllowMarble();
      } else {
        RejectMarble();
      }
    }
    if (Policy == 3) {
      AllowMarble();
    }
    if (Policy == 4) {
      RejectMarble();
    }
    state = sMeasuring;
  }
}

/*
 * Get default value and fill raw array initially
 */
int calibrate(int calibrateMs) {
  long samplesTotal = 0;
  for (int i = 0; i < SAMPLES; i++) {
    int rawValue = analogRead(SensorPin);
    samplesTotal += rawValue;
    rawArray[i] = rawValue;
    delay(calibrateMs/SAMPLES);
  }
  return samplesTotal/SAMPLES;
}

int getAverage(int *rawValues) {
  long samplesTotal = 0;
  for (int i = 0; i < SAMPLES; i++) {
    samplesTotal += rawValues[i];
  }
  return samplesTotal/SAMPLES;
}

void AllowMarble() {
    Serial.println("allow");
    myServo.write(servo_allow);
    delay(1000);
    myServo.write(servo_rest);
    //transmitCAN(messagePassed);
    //IsAllowedToPass = false;
    //state = Wait;
    delay(500);
    digitalWrite(13, LOW);
}

void RejectMarble() {
    Serial.println("reject");
    myServo.write(servo_reject);
    delay(1000);
    myServo.write(servo_rest);
    //transmitCAN(messageRejected);
    //state = Wait;
    delay(500);
    digitalWrite(13, LOW);
}

// the fuck?
bool SetPolicy() {
    CANMSG canReceived;
    CustomCanMessage can_message;
    if (can.receiveCANMessage(&canReceived, CAN_MS_TIMEOUT) &&
    ParseMessage(canReceived, can_message)) {
        if(can_message.senderAddress == CAN_Address_Server) {
            CustomCanServerMessage* policy = (CustomCanServerMessage*)&can_message;
            
            PrintMessage(can_message);

            Policy = policy->policy_hoogte;
            return true;    
        }
        else {
            if(can_message.receiverAddress == CAN_MyAddress) {
                //received message addresed to me
            }
            else if(can_message.receiverAddress == CAN_Address_Broadcast) {
                //received broadcast message
                if(can_message.senderAddress == CAN_Address_Color &&
                (can_message.function == BROADCAST_MARBLE_ACCEPTED ||
                can_message.function == BROADCAST_MARBLE_ACCEPTED)){
                    //IsAllowedToPass = true;
                }                
            }
        }
    }
    return false;
}

void PrintMessage(CustomCanMessage& message) {
    Serial.print(message.receiverAddress,HEX);
    Serial.print(',');
    Serial.print(message.senderAddress,HEX);
    Serial.print(',');
    Serial.print(message.function,HEX);
    Serial.print(',');
    Serial.print(message.measure,HEX);
    Serial.print(',');
    Serial.print(message.diagnostics,HEX);
    Serial.print(',');
    Serial.print(message.empty1,HEX);
    Serial.print(',');
    Serial.print(message.empty2,HEX);
    Serial.print(',');
    Serial.print(message.empty3,HEX);
    Serial.print("");
}

