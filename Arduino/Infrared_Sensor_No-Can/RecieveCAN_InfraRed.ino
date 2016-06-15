

bool SetPolicy()
{
  CANMSG canReceived;
  if (can.receiveCANMessage(&canReceived, canReceiveTimeoutMs))\
  {
    SerialMessage message = ParseMessage(canReceived);
    if (message.module2 == CAN_MyAddress)
    {
      Policy = message.policy1;
      return true;
    }
  }
  return false;
}

SerialMessage ParseMessage(CANMSG message)
{
  SerialMessage msg;
  msg.senderAddress = message.adrsValue;
  msg.module1 = message.data[0];
  msg.policy1 = message.data[1];
  msg.module2 = message.data[2];
  msg.policy2 = message.data[3];
  msg.module3 = message.data[4];
  msg.policy3 = message.data[5];
  msg.empty = message.data[6];
  return msg;
}
