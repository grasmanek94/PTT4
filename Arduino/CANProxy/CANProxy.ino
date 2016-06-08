#include <SPI.h>
#include <MCP2515.h>
#define SERIALBUFF_SZ 64

#define CAN_MyAddress 0x0A

MCP2515 can;
CANMSG canReceived;
int canReceiveTimeoutMs = 100;
int canTransmitTimeoutMs = 100;

byte startSign = '[';
byte endSign = ']';

byte serialBuffer[SERIALBUFF_SZ] = {};
int serialBufferPos = 0;

struct SerialMessage {
  byte senderAddress;
  byte module1;
  byte policy1;
  byte module2;
  byte policy2;
  byte module3;
  byte policy3;
  byte empty;
};

void setup() {
  Serial.begin(9600); 
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  if(can.initCAN(CAN_BAUD_100K) == 0) {
    Serial.println("initCAN() failed");
    while(1);
  }
  if(can.setCANNormalMode(LOW) == 0) { //normal mode non single shot
    Serial.println("setCANNormalMode() failed");
    while(1);
  }
}

void loop() {
  // incoming CAN message processing
  handleCAN(can.receiveCANMessage(&canReceived, canReceiveTimeoutMs));
  
  // incoming Serial message processing
  SerialMessage serialMsg;
  if (handleSerial(&serialMsg)) {
    if (transmitCAN(serialMsg)) {
      Serial.println("Can Transmit success");
    }
  }
}

bool transmitCAN(SerialMessage message) {
  CANMSG canSendMsg;
  canSendMsg.adrsValue = CAN_MyAddress;
  canSendMsg.isExtendedAdrs = false;
  canSendMsg.rtr = false;
  canSendMsg.dataLength = 8;
  canSendMsg.data[0]=message.module1;
  canSendMsg.data[1]=message.policy1;
  canSendMsg.data[2]=message.module2;
  canSendMsg.data[3]=message.policy2;
  canSendMsg.data[4]=message.module3;
  canSendMsg.data[5]=message.policy3;
  canSendMsg.data[6]=message.empty;
  canSendMsg.data[7]=message.empty;
  can.transmitCANMessage(canSendMsg, canTransmitTimeoutMs);
}

bool handleSerial(SerialMessage *message) {
  byte currByte = 0;
  if (Serial.available() > 0) {
    currByte = Serial.read();
    serialBuffer[serialBufferPos] = currByte;
    serialBufferPos++;
  }
  if (currByte == endSign) {
    int beginPos = -1;
    for (int i = 0; i < serialBufferPos; i++) {
      if (serialBuffer[i] == beginPos) {
        beginPos = i;
      }
    }
    if (beginPos == -1) {
      memset(serialBuffer, 0, SERIALBUFF_SZ);
      serialBufferPos = 0;
      return false;
    } else {
      message->senderAddress  = serialBuffer[serialBufferPos+1];
      message->module1        = serialBuffer[serialBufferPos+2];
      message->policy1        = serialBuffer[serialBufferPos+3];
      message->module2        = serialBuffer[serialBufferPos+4];
      message->policy2        = serialBuffer[serialBufferPos+5];
      message->module3        = serialBuffer[serialBufferPos+6];
      message->policy3        = serialBuffer[serialBufferPos+7];
      message->empty          = 0;
      memset(serialBuffer, 0, SERIALBUFF_SZ);
      serialBufferPos = 0;
      return true;
    }
  }
  return false;
}

void handleCAN(bool messageReceived) {
  sendSerialMsg(canReceived);
}

void sendSerialMsg(CANMSG message) {
  byte dataToSend[8] = {};

}

void sendCanMsg(unsigned long adrsValue, int dataLength, byte *data) {
  CANMSG msg;
  msg.adrsValue = adrsValue;
  msg.isExtendedAdrs = false;
  msg.rtr = false;
  msg.dataLength = dataLength;
  for (int i = 0; i < dataLength; i++) {
    msg.data[i] = data[i];
  }
  can.transmitCANMessage(msg, 1000);
  Serial.println("data transmitted");
}

