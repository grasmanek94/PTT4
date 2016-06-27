#include <Servo.h>
#include <Arduino.h>
#include <MCP2515.h>
#include "canx.h"

class MarblePassThrough
{
    long time_start;
    long time_end;
    int pos_start;
    int pos_now;
    int pos_end;
    Servo servo;
    bool move_state;
    long move_time;
    int servo_pin;
        
    int detector_pin;
    long last_detect_time;
    long detect_time;

    bool getting_marble;

    bool CheckPosition()
    {
         long time_now = millis();
        if(time_now >= time_start && time_now <= time_end)
        {
            long time_diff = time_now - time_start;
            float percentage = ((float)time_diff) / ((float)(move_time));
            int pos_calculated = pos_start + (int)(((float)(pos_end - pos_start)) * percentage);
            if(pos_calculated != pos_now)
            {
                pos_now = pos_calculated;
                servo.write(pos_now);
            }
            
            return false;
        }
        else
        {
            servo.write(pos_end);
            return true;
        }       
    }

    void PassMarble()
    {
        pos_start = 107;
        pos_end = 0;
        time_start = millis();
        time_end = time_start + move_time;
        move_state = false;
        Serial.println("Pass Marble");
    }

    void LoadMarble()
    {
        pos_start = 0;
        pos_end = 107;   
        time_start = millis();
        time_end = time_start + move_time;      
        move_state = true;    
        Serial.println("Load Marble");   
    }

    void Switch()
    {
        Serial.println("SWITCH - Lift START Requested");
        transmitCAN(messageLiftStart);
        if(!move_state)
        {
            LoadMarble();
        }
        else
        {
            PassMarble();
        }
    }
    
    bool GetMoveState()
    {
        return move_state;
    }

    bool DetectedMarble()
    {
        long time_now = millis();
        if(time_now < (last_detect_time + detect_time))
        {
            return false;
        }
        
        bool ballPassed = digitalRead(detector_pin);        
        if(ballPassed)
        {
            return false;
        }

        Serial.println("DETECTED");
        last_detect_time = time_now;
        return true;       
    }
          
public:
    MarblePassThrough(int servoPin, int detectorPin)
    {
        detector_pin = detectorPin;
        servo_pin = servoPin;
        move_state = true;      
        move_time = 1000;
        detect_time = 1000;
        last_detect_time = 0;   
        pos_now = 0;    
        getting_marble = false;    
    }

    ~MarblePassThrough()
    {
        
    }

    void Setup()
    {
        pinMode(detector_pin, INPUT_PULLUP);   
        servo.attach(servo_pin);
             
        LoadMarble();
    }

    bool GettingMarbleTick()
    {
        if(getting_marble)
        {
            if(CheckPosition())
            {
                Switch();
            }

            if(DetectedMarble())
            {
                StopMarble();
                return true;
            }            
        }
        return false;
    }

    void GetMarble()
    {
        if(!getting_marble)
        {
            getting_marble = true;
            transmitCAN(messageLiftStart);
            Serial.println("START - Lift START Requested");
        }
    }

    void StopMarble()
    {
        if(getting_marble)
        {
            getting_marble = false;
            transmitCAN(messageLiftStop);
            Serial.println("STOP - Lift STOP Requested");
        }
    }

    bool GettingMarble()
    {
        return getting_marble;
    }
};

MarblePassThrough passer(4, 2);
bool lightning = false;
bool enabled_module = true;

bool ProcessIncommingMessages()
{
    CANMSG canReceived;
    CustomCanMessage smsg;
    if (can.receiveCANMessage(&canReceived, CAN_MS_TIMEOUT) && ParseMessage(canReceived, smsg))
    {
        Serial.print("Received message from");
        Serial.println(smsg.senderAddress);
        if(smsg.senderAddress == CAN_Address_Server)
        {
            CustomCanServerMessage* policy = (CustomCanServerMessage*)&smsg;
            if(policy->policy_kleur == 5 || policy->policy_hoogte == 4 || policy->policy_transparantie == 4) // niets
            {
                enabled_module = false;
                passer.StopMarble();
                Serial.println("Doorlaat Module Stopped - Any policy == 'niets'");
            }
            else
            {
                enabled_module = true;
                Serial.println("Doorlaat Module enabled - None policy == 'niets'");
            }
        }
        else if(enabled_module)
        {
            if (smsg.receiverAddress == CAN_MyAddress)
            {
                passer.GetMarble();
            }
            else if (smsg.receiverAddress == CAN_Address_Broadcast)
            {
                switch(smsg.function)
                {
                    case BROADCAST_MARBLE_ACCEPTED:
                        if(smsg.senderAddress == CAN_Address_Transparency)
                        {
                            passer.GetMarble();
                        }
                    break;
                    case BROADCAST_MARBLE_REJECTED:
                        passer.GetMarble();
                    break;
                }
            }
        }
    }
    return false;
}

void setup() 
{
    Serial.begin(9600);
    Serial.println("SWEEPER ONLINE");
    pinMode(13, OUTPUT);
    InitCan();
    
    passer.Setup();
    passer.GetMarble();
}

long timeLast = 0;
void loop() 
{
    if(enabled_module)
    {
        if(passer.GettingMarbleTick())
        {
            lightning ^= 1;
            digitalWrite(13, lightning);
            Serial.println("Message Passed");
            transmitCAN(messagePassed);
        }
    }
    else
    {
        long timeNow = millis();
        digitalWrite(13, ((timeNow / 250) % 2));
        if(timeNow - timeLast > 1000)
        {
            timeLast = timeNow;
            transmitCAN(messageLiftStop);
            Serial.println("TIMER - Lift STOP Requested");
        }
    }

    ProcessIncommingMessages();
}

