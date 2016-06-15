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

SerialMessage CreateMessageTrue()
{
  SerialMessage msg;
  msg.senderAddress = 0XFF;
  msg.module1 = 0xFF;
  msg.policy1 = 0xFF;
  msg.module2 = 0xFF;
  msg.policy2 = 0xFF;
  msg.module3 = 0xFF;
  msg.policy3 = 0xFF;
  msg.empty = 0x00;
  return msg;
}

SerialMessage CreateMessageFalse()
{
  SerialMessage msg;
  msg.senderAddress = 0XAA;
  msg.module1 = 0xAA;
  msg.policy1 = 0xAA;
  msg.module2 = 0xAA;
  msg.policy2 = 0xAA;
  msg.module3 = 0xAA;
  msg.policy3 = 0xAA;
  msg.empty = 0x00;
  return msg;
}

bool transmitCAN(SerialMessage message) {

  CANMSG canmsg;
  canmsg.adrsValue = CAN_MyAddress;
  canmsg.isExtendedAdrs = false;
  canmsg.rtr = false;
  canmsg.dataLength = 8;
  canmsg.data[0] = message.module1;
  canmsg.data[1] = message.policy1;
  canmsg.data[2] = message.module2;
  canmsg.data[3] = message.policy2;
  canmsg.data[4] = message.module3;
  canmsg.data[5] = message.policy3;
  canmsg.data[6] = message.empty;
  canmsg.data[7] = message.empty;
  can.transmitCANMessage(canmsg, canTransmitTimeoutMs);
}

