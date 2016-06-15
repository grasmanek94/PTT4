#include <Servo.h>
#include <Arduino.h>
#include <MCP2515.h>


///////////////////////////////////
struct CustomCanMessage
{
    byte senderAddress;
    byte receiverAddress;
    byte function;
    byte measure;
    byte diagnostics;
    byte empty1;
    byte empty2;
    byte empty3;
};

#define CAN_Address_Invalid 0x00
#define CAN_Address_Sweeper 0x01
#define CAN_Address_Infrared 0x02
#define CAN_Address_Lift 0x03
#define CAN_Address_Proxy 0x04
#define CAN_Address_Color 0x05
#define CAN_Address_Transparency 0x06
#define CAN_Address_Server 0x07
#define CAN_Address_Broadcast 0xFF

#define CAN_MyAddress CAN_Address_Sweeper

#define CAN_MS_TIMEOUT 20

#define LIFT_START 0x01
#define LIFT_STOP 0x02

#define BROADCAST_SWEEPER_MARBLE_PASSED 0x01
#define BROADCAST_MARBLE_ACCEPTED 0x01
#define BROADCAST_MARBLE_REJECTED 0x02
#define BROADCAST_MARBLE_GET_NEXT 0x03

MCP2515 can;

void InitCan()
{
    if (can.initCAN(CAN_BAUD_100K) == 0) 
    {
        Serial.println("initCAN() failed");
        while (1);
    }
    if (can.setCANNormalMode(LOW) == 0) 
    { //normal mode non single shot
        Serial.println("setCANNormalMode() failed");
        while (1);
    }
}

CustomCanMessage messageLiftStop {CAN_MyAddress, CAN_Address_Lift, LIFT_STOP, 0, 0, 0, 0, 0};
CustomCanMessage messageLiftStart {CAN_MyAddress, CAN_Address_Lift, LIFT_START, 0, 0, 0, 0, 0};
CustomCanMessage messageBroadcastMarblePassed {CAN_MyAddress, CAN_Address_Broadcast, BROADCAST_SWEEPER_MARBLE_PASSED, 0, 0, 0, 0, 0};

bool ParseMessage(CANMSG& message, CustomCanMessage& msg)
{
    msg.senderAddress = message.adrsValue;
    msg.senderAddress = message.data[0];
    msg.receiverAddress = message.data[1];
    msg.function = message.data[2];
    msg.measure = message.data[3];
    msg.diagnostics = message.data[4];
    msg.empty1 = message.data[5];
    msg.empty2 = message.data[6];
    msg.empty3 = message.data[7];
    return true;
}

bool transmitCAN(CustomCanMessage& message) 
{
    CANMSG canmsg;
    canmsg.adrsValue = CAN_MyAddress;
    canmsg.isExtendedAdrs = false;
    canmsg.rtr = false;
    canmsg.dataLength = 8;
    canmsg.data[0] = message.senderAddress;
    canmsg.data[1] = message.receiverAddress;
    canmsg.data[2] = message.function;
    canmsg.data[3] = message.measure;
    canmsg.data[4] = message.diagnostics;
    canmsg.data[5] = message.empty1;
    canmsg.data[6] = message.empty2;
    canmsg.data[7] = message.empty3;
    can.transmitCANMessage(canmsg, CAN_MS_TIMEOUT);
}

///////////////////////////////////

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
    }

    void LoadMarble()
    {
        pos_start = 0;
        pos_end = 107;   
        time_start = millis();
        time_end = time_start + move_time;      
        move_state = true;       
    }

    void Switch()
    {
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
                getting_marble = false;
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
        }
    }

    void StopMarble()
    {
        if(getting_marble)
        {
            getting_marble = false;
            transmitCAN(messageLiftStop);
        }
    }

    bool GettingMarble()
    {
        return getting_marble;
    }
};

MarblePassThrough passer(4, 2);
bool lightning = false;

bool ProcessIncommingMessages()
{
    CANMSG canReceived;
    CustomCanMessage smsg;
    if (can.receiveCANMessage(&canReceived, CAN_MS_TIMEOUT) && ParseMessage(canReceived, smsg))
    {
        if (smsg.receiverAddress == CAN_MyAddress)
        {
            passer.GetMarble();
        }
        else if (smsg.receiverAddress == CAN_Address_Broadcast)
        {
        
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

    //Get one marble
    passer.GetMarble();
}

void loop() 
{
    if(passer.GettingMarbleTick())
    {
        lightning ^= 1;
        digitalWrite(13, lightning);
        Serial.println("DM");
        transmitCAN(messageBroadcastMarblePassed);
    }

    ProcessIncommingMessages();
    if(!passer.GettingMarble())
    {
        delay(5000);
        passer.GetMarble();
    }
}

