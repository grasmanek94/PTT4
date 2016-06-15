#include "motorshield.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "canx.h"

// 1 rotation = 1300 pulses (approx)

#define INT_PIN 3

DualVNH5019MotorShield motorController;

long lastTime;
long printLastTime;

volatile long pulseCount;
volatile long pulsesPerSecond;

volatile int timer2Count;

/*ISR(TIMER2_COMPA_vect)
{
  ++timer2Count;               
  if(timer2Count >= 1000)
  {
    pulsePerSecond = pulseCount;
    pulseCount = 0;
  }

  
};*/ 
bool enabled_module = true;
int lift_state; //0 disabled 1 running 2 stopped
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
    // set up timer2
    
    /*
    cli();
    
    TCCR2A = _BV(WGM21); // CTC mode
    TCCR2B = _BV(); // clock selection bits for prescaler
    
    TCNT2 = 0; // reset timer counter
    TIMSK2 = 0;
    TIMSK2 |= _BV(OCIE2A); // interrupt on output compare register A
    
    sei();*/
    
    pinMode(INT_PIN, INPUT_PULLUP);
    motorController.init();
    
    motorController.setSpeeds(0,0);
    
    attachInterrupt(digitalPinToInterrupt(INT_PIN), onPulse, CHANGE);
    
    Serial.begin(9600);
    InitCan();
    
    lastTime = millis();
    printLastTime = millis();
}

/*
 * /*How long since we last calculated
   unsigned long now = millis();
   double timeChange = (double)(now - lastTime);
  
   /*Compute all the working error variables
   double error = Setpoint - Input;
   errSum += (error * timeChange);
   double dErr = (error - lastErr) / timeChange;
  
   /*Compute PID Output
   Output = kp * error + ki * errSum + kd * dErr;
  
   /*Remember some variables for next time
   lastErr = error;
   lastTime = now;
 */

int targetSpeed = 1200;

#define MAX_SPEED 1300
#define MIN_POWER 250
#define MAX_POWER 400

void loop() 
{
    ProcessIncommingMessages();
    if(lift_state == 1)
    {
        long now = micros();
        int deltaTime = now - lastTime;
        lastTime = now;
   
        //double pulsesPerMicro = totalPulses / deltaTime;
        //int pulsesPerSecond = pulsesPerMicro * 1000000;
        
        if ((millis() - printLastTime) > 100)
        {
            cli();
            int totalPulses = pulseCount;
            pulseCount = 0;
            sei();
            
            Serial.print("Current Speed: ");
            Serial.println(totalPulses);
            printLastTime = millis();
        }
    
        /*
        double error = targetSpeed - currentSpeed;
        double kp = 1;
        double newSpeed = kp * error;
        
        double speedFactor = newSpeed / MAX_SPEED;
        double power = speedFactor * MAX_POWER;
        
        if ((int)power > MAX_POWER)
        {
        power = MAX_POWER;
        }
        else if ((int)power < MIN_POWER)
        {
        power = MIN_POWER;
        }
        
        Serial.print(" Power: ");
        Serial.println(power);*/

        //motorController.setSpeeds((int)power, (int)power);
        motorController.setSpeeds((int)400, (int)400);
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


