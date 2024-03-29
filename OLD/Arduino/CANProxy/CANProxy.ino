#include <SPI.h>
#include <MCP2515.h> //modified - public readReg(byte regno)
#define SERIALBUFF_SZ 64

#define CAN_MyAddress 0x0A

MCP2515 can;
CANMSG canReceived;
int canReceiveTimeoutMs = 20;
int canTransmitTimeoutMs = 20;

byte startSign = '[';
byte endSign = ']';

bool ledStatus13 = true;

byte serialBuffer[SERIALBUFF_SZ] = {};
int serialBufferPos = 0;

struct CustomSerialMessage {
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
  pinMode(3, OUTPUT);
  Serial.begin(9600); 
  while (!Serial) ; 
  Serial.println("Go...");

  
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  if(can.initCAN(CAN_BAUD_100K) == 0) {
    Serial.println("initCAN() failed");
    while(1);
  }
  if(can.setCANNormalMode(LOW) == 0) { //normal mode non single shot
    Serial.println("setCANNormalMode() failed");
    while(1);
  }
  Serial.println("setup() success");
}

void loop() {
  digitalWrite(3, ledStatus13);

  
  // incoming CAN message processing
  // Stole this from the receiveCANMessage method in MCP2515.cpp
  // Should trigger right when a new CAN message is available now
  byte val = can.readReg(0x2C); //CANINTF
  if (bitRead(val, 0) == 1) {
    handleCAN(can.receiveCANMessage(&canReceived, canReceiveTimeoutMs));
  }
    
  // incoming Serial message processing
  CustomSerialMessage serialMsg;
  if (handleSerial(&serialMsg)) {
    if (transmitCAN(serialMsg)) {
      Serial.println("CAN Transmit success");
    } else {
      Serial.println("CAN Transmit failed");
    }
  }
  
}

bool transmitCAN(CustomSerialMessage message) {
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
  Serial.print("Msg: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(canSendMsg.data[i]);
    Serial.print(" ");
  }
  return can.transmitCANMessage(canSendMsg, canTransmitTimeoutMs);
}

bool handleSerial(CustomSerialMessage *message) {
  byte currByte = 0;
  if (Serial.available() > 0) {
    currByte = Serial.read();
    serialBuffer[serialBufferPos] = currByte;
    serialBufferPos++;
  }
  if (currByte == endSign) {
    int beginPos = -1;
    for (int i = 0; i < serialBufferPos; i++) {
      if (serialBuffer[i] == startSign) {
        beginPos = i;
      }
    }
    if (beginPos != -1 && (serialBufferPos-1) - (beginPos+1) == 8) {
      message->senderAddress  = serialBuffer[beginPos+1];
      message->module1        = serialBuffer[beginPos+2];
      message->policy1        = serialBuffer[beginPos+3];
      message->module2        = serialBuffer[beginPos+4];
      message->policy2        = serialBuffer[beginPos+5];
      message->module3        = serialBuffer[beginPos+6];
      message->policy3        = serialBuffer[beginPos+7];
      message->empty          = 0;
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

void handleCAN(bool messageReceived) {
  sendSerialMsg(canReceived);
}

void sendSerialMsg(CANMSG message) {
  byte dataToSend[8] = {}; //wip pls fix
  dataToSend[0] = message.adrsValue;
  dataToSend[1] = message.data[0];
  dataToSend[2] = message.data[1];
  dataToSend[3] = message.data[2];
  dataToSend[4] = message.data[3];
  dataToSend[5] = message.data[4];
  dataToSend[6] = message.data[5];
  dataToSend[7] = message.data[6];

  Serial.write(startSign);
  for (int i = 0; i < 8; i++) {
    Serial.write(dataToSend[i]);
  }
  Serial.write(endSign);
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

