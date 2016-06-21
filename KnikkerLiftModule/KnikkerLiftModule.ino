#include <avr/interrupt.h>
#include <avr/io.h>
#include "motorshield.h"
#include "canx.h"
#include "pid.h"

// 1 rotation = 535 pulses (approx)

#define INT_PIN 3

DualVNH5019MotorShield motorController;

volatile long pulseCount;
volatile long pulsesPerSecond;

bool enabled_module = true;
int lift_state = 1; //0 disabled 1 running 2 stopped
bool ProcessIncommingMessages()
{
    CANMSG canReceived;
    CustomCanMessage smsg;
    if (can.receiveCANMessage(&canReceived, CAN_MS_TIMEOUT) && ParseMessage(canReceived, smsg))
    {
        if(smsg.senderAddress == CAN_Address_Server)
        {
            CustomCanServerMessage* policy = (CustomCanServerMessage*)&smsg;
            if(policy->policy_kleur == 5 || policy->policy_hoogte == 4 || policy->policy_transparantie == 4) // niets
            {
                enabled_module = false;
                lift_state = 0;
            }
            else
            {
                enabled_module = true;
                lift_state = 2;
            }
        }
        else if(enabled_module)
        {
            if (smsg.receiverAddress == CAN_MyAddress)
            {
                switch(smsg.function)
                {
                    case LIFT_START:
                    lift_state = 1;
                    break;

                    case LIFT_STOP:
                    lift_state = 2;
                    break;
                }
            }
            else if (smsg.receiverAddress == CAN_Address_Broadcast)
            {
 
            }
        }
    }
    return false;
}

void setup() 
{
    pinMode(INT_PIN, INPUT_PULLUP);
    motorController.init();
    
    motorController.setSpeeds(0,0);
    
    attachInterrupt(digitalPinToInterrupt(INT_PIN), onPulse, CHANGE);
    
    Serial.begin(9600);
    InitCan();
}

long timeLast = 0;

const double MIN_ROTATIONS = -540.0;
const double MAX_ROTATIONS = 540.0;
const double TARGET_ROTATIONS = 350.0;

const double PID_KP = 1.0;
const double PID_KI = 1.0;
const double PID_KD = 1.0;

const double MIN_POWER = 260.0;
const double MAX_POWER = 400.0;

PID pid_controller(MIN_ROTATIONS, MAX_ROTATIONS, PID_KP, PID_KD, PID_KI);

void loop() 
{
    ProcessIncommingMessages();
    lift_state = 1;
    if(lift_state == 1)
    {
        long timeNow = millis();
        long deltaTime = timeNow - timeLast;

        if (deltaTime > 100)
        {             
            cli();
            double totalPulses = (double)pulseCount;
            pulseCount = 0;
            sei();

            double dt_seconds = (double)deltaTime / 1000.0;
            timeLast = timeNow;
            double value_to_set = -1.0 * pid_controller.calculate(TARGET_ROTATIONS, totalPulses, dt_seconds);
            double motor_speed = MIN_POWER + ((value_to_set/(MAX_ROTATIONS - MIN_ROTATIONS)) * (MAX_POWER - MIN_POWER));
            Serial.print("MS:");
            Serial.print(motor_speed);
            Serial.print("VS:");
            Serial.print(value_to_set);
            Serial.print("TP:");                
            Serial.println(totalPulses);
            motorController.setSpeeds((int)motor_speed, (int)motor_speed);
        }
 
        //motorController.setSpeeds((int)power, (int)power);
        //motorController.setSpeeds((int)400, (int)400);
    }
    else
    {        
        motorController.setSpeeds(0, 0);
    }
}

void onPulse()
{
  ++pulseCount;
}


