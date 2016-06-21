#include "canx.h"

#define SERIALBUFF_SZ 64

CANMSG canReceived;
CustomCanServerMessage serialMsg;

byte startSign = '[';
byte endSign = ']';

bool ledStatus13 = true;

byte serialBuffer[SERIALBUFF_SZ] = {};
int serialBufferPos = 0;

void setup() 
{
    pinMode(3, OUTPUT);
    Serial.begin(9600); 
    while (!Serial) ; 
    Serial.println("Go...");
    
    InitCan();
    
    Serial.println("setup() success");
}

void loop() 
{
    digitalWrite(3, ledStatus13);
    
    // incoming CAN message processing
    // Stole this from the receiveCANMessage method in MCP2515.cpp
    // Should trigger right when a new CAN message is available now
    byte val = can.readReg(0x2C); //CANINTF
    if (bitRead(val, 0) == 1)
    {
        if(can.receiveCANMessage(&canReceived, CAN_MS_TIMEOUT))
        {
            sendSerialMsg();
        }
    }
    
    // incoming Serial message processing
    if (handleSerial(&serialMsg)) 
    {
        transmitCAN(serialMsg);
    }
}

bool handleSerial(CustomCanServerMessage *message) 
{
    byte currByte = 0;
    
    if (Serial.available() > 0) 
    {
        currByte = Serial.read();
        serialBuffer[serialBufferPos] = currByte;
        serialBufferPos++;
    }
    
    if (currByte == endSign) 
    {
        int beginPos = -1;
        for (int i = 0; i < serialBufferPos; i++) 
        {
            if (serialBuffer[i] == startSign) 
            {
                beginPos = i;
            }
        }
        if (beginPos != -1 && (serialBufferPos-1) - (beginPos+1) == 8)
        {
            message->senderAddress        = serialBuffer[beginPos+1];
            message->receiverAddress      = serialBuffer[beginPos+2];
            message->module_kleur         = serialBuffer[beginPos+3];
            message->policy_kleur         = serialBuffer[beginPos+4];
            message->module_hoogte        = serialBuffer[beginPos+5];
            message->policy_hoogte        = serialBuffer[beginPos+6];
            message->module_transparantie = serialBuffer[beginPos+7];
            message->policy_transparantie = serialBuffer[beginPos+8];

            memset(serialBuffer, 0, SERIALBUFF_SZ);
            serialBufferPos = 0;
            Serial.println("Serial msg valid!");
            ledStatus13 = !ledStatus13;
            return true;
        } else {
            memset(serialBuffer, 0, SERIALBUFF_SZ);
            serialBufferPos = 0;
            Serial.println("Serial msg invalid: No begin char or wrong leng");
            return false;
        }
        Serial.println("Serial msg invalid: Something!");
    }
    return false;
}

void sendSerialMsg() 
{  
    Serial.write(startSign);
    for (int i = 0; i < 8; i++) 
    {
        Serial.write(canReceived.data[i]);
    }
    Serial.write(endSign);
}

