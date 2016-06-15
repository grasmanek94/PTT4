#include <MCP2515.h>

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

#define LIFT_START 0x01
#define LIFT_STOP 0x02

#define BROADCAST_SWEEPER_MARBLE_PASSED 0x01
#define BROADCAST_MARBLE_ACCEPTED 0x01
#define BROADCAST_MARBLE_REJECTED 0x02
#define BROADCAST_MARBLE_GET_NEXT 0x03

MCP2515 can;

struct SerialMessage 
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

SerialMessage messageLiftStop {CAN_MyAddress, CAN_Address_Lift, LIFT_STOP, 0, 0, 0, 0, 0};
SerialMessage messageLiftStart {CAN_MyAddress, CAN_Address_Lift, LIFT_START, 0, 0, 0, 0, 0};
SerialMessage messageBroadcastMarblePassed {CAN_MyAddress, CAN_Address_Broadcast, BROADCAST_SWEEPER_MARBLE_PASSED, 0, 0, 0, 0, 0};

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
/*
bool transmitCAN(SerialMessage& message) {

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
  can.transmitCANMessage(canmsg, canTransmitTimeoutMs);
}*/

