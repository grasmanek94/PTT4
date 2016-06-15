

bool SetPolicy()
{
  CANMSG canReceived;
  if (can.receiveCANMessage(&canReceived, canReceiveTimeoutMs))
  {
    Serial.println("Message");
    CustomCanMessage message = ParseMessage(canReceived);
    PrintMessage(message);
    if (message.data3 == CAN_Address_Infrared && message.senderAddress == CAN_Address_Server)
    {
      Policy = message.data4;
      return true;
    }
    if(message.senderAddress == CAN_Address_Color)
    {
      if(message.data6 == 0xFF || message.data6 == 0xAA)
      {
        IsAllowedToPass = true;
      }
    }
  }
  return false;
}

CustomCanMessage ParseMessage(CANMSG message)
{
  CustomCanMessage msg;
  msg.senderAddress = message.adrsValue;
  msg.data1 = message.data[0];
  msg.data2 = message.data[1];
  msg.data3 = message.data[2];
  msg.data4 = message.data[3];
  msg.data5 = message.data[4];
  msg.data6 = message.data[5];
  msg.empty = message.data[6];
  return msg;
}

void PrintMessage(CustomCanMessage message)
{
  Serial.print(message.senderAddress,HEX);
  Serial.print(',');
  Serial.print(message.data1,HEX);
  Serial.print(',');
  Serial.print(message.data2,HEX);
  Serial.print(',');
  Serial.print(message.data3,HEX);
  Serial.print(',');
  Serial.print(message.data4,HEX);
  Serial.print(',');
  Serial.print(message.data5,HEX);
  Serial.print(',');
  Serial.print(message.data6,HEX);
  Serial.print(',');
  Serial.print(message.empty,HEX);
  Serial.print("");
}

