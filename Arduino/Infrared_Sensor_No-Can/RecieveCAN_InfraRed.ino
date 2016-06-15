

bool SetPolicy()
{
  CANMSG canReceived;
  if (can.receiveCANMessage(&canReceived, canReceiveTimeoutMs))
  {
    Serial.println("Message");
    CustomCanMessage message = ParseMessage(canReceived);
    PrintMessage(message);
    if (message.module2 == CAN_MyAddress)
    {
      Policy = message.policy2;
      return true;
    }
  }
  return false;
}

CustomCanMessage ParseMessage(CANMSG message)
{
  CustomCanMessage msg;
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

void PrintMessage(CustomCanMessage message)
{
  Serial.print(message.senderAddress,HEX);
  Serial.print(',');
  Serial.print(message.module1,HEX);
  Serial.print(',');
  Serial.print(message.policy1,HEX);
  Serial.print(',');
  Serial.print(message.module2,HEX);
  Serial.print(',');
  Serial.print(message.policy2,HEX);
  Serial.print(',');
  Serial.print(message.module3,HEX);
  Serial.print(',');
  Serial.print(message.policy3,HEX);
  Serial.print(',');
  Serial.print(message.empty,HEX);
  Serial.print("");
}

