///////////////////////////////////
/*
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
*/
#define CAN_Address_Invalid 0x00 // niemand / niks
#define CAN_Address_Sweeper 0x01 // module van Rafal 
#define CAN_Address_Infrared 0x02 // module van Mathieu
#define CAN_Address_Lift 0x03 // modulfe van Thomas & Raf
#define CAN_Address_Proxy 0x04 // module van Minhtriet
#define CAN_Address_Color 0x05 // module van Coen
#define CAN_Address_Transparency 0x06 // module van Thomas
#define CAN_Address_Server 0x07 // module van X
#define CAN_Address_Broadcast 0xFF // module van iedereen

#define CAN_MyAddress CAN_Address_Sweeper

#define CAN_MS_TIMEOUT 20

#define LIFT_START 0x01
#define LIFT_STOP 0x02

#define BROADCAST_SWEEPER_MARBLE_PASSED 0x01
#define BROADCAST_MARBLE_ACCEPTED 0x01
#define BROADCAST_MARBLE_REJECTED 0x02
#define BROADCAST_STOP_ALL 0x03

/*MCP2515 can;

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
*/
///////////////////////////////////
