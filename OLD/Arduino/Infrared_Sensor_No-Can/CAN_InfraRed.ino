#include <MCP2515.h>

void InitCan()
{
  if (can.initCAN(CAN_BAUD_100K) == 0) {
    Serial.println("initCAN() failed");
    while (1);
  }
  if (can.setCANNormalMode(LOW) == 0) { //normal mode non single shot
    Serial.println("setCANNormalMode() failed");
    while (1);
  }
}

CustomCanMessage CreateMessageTrue()
{
  CustomCanMessage msg;
  msg.senderAddress = CAN_Address_Infrared;
  msg.data1 = CAN_Address_Broadcast;
  msg.data2 = BROADCAST_MARBLE_ACCEPTED;
  msg.data3 = 0x00;
  msg.data4 = 0x00;
  msg.data5 = 0x00;
  msg.data6 = 0x00;
  msg.empty = 0x00;
  return msg;
}

CustomCanMessage CreateMessageFalse()
{
  CustomCanMessage msg;
  msg.senderAddress = CAN_Address_Infrared;
  msg.data1 = CAN_Address_Broadcast;
  msg.data2 = BROADCAST_MARBLE_REJECTED;
  msg.data3 = 0x00;
  msg.data4 = 0x00;
  msg.data5 = 0x00;
  msg.data6 = 0x00;
  msg.empty = 0x00;
  return msg;
}

bool transmitCAN(CustomCanMessage message) {

  CANMSG canmsg;
  canmsg.adrsValue = CAN_Address_Infrared;
  canmsg.isExtendedAdrs = false;
  canmsg.rtr = false;
  canmsg.dataLength = 8;
  canmsg.data[0] = message.senderAddress;
  canmsg.data[1] = message.data1;
  canmsg.data[2] = message.data2;
  canmsg.data[3] = message.data3;
  canmsg.data[4] = message.data4;
  canmsg.data[5] = message.data5;
  canmsg.data[6] = message.data6;
  canmsg.data[7] = message.empty;
  can.transmitCANMessage(canmsg, canTransmitTimeoutMs);
}

