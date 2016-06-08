using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace KnikkerBaanServer
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in both code and config file together.
    public class CScadaService : IScadaService
    {
        ArduinoCommunicator arduinoCommunicator = new ArduinoCommunicator("COM4", 9600);
        MessageStorage messageStorage = MessageStorage.Messagestorage;
        public void SetPolicy(Policy policy)
        {
            byte[] message = new byte[10];
            int i = 0;
            for (; i < 10; i++)
            {
                switch (i)
                {
                    case 0:
                        message[i] = (byte)arduinoCommunicator.messageStart;
                        break;
                    case 1:
                        message[i] = policy.address;
                        break;
                    case 2:
                        message[i] = policy.PolicyModuleOne;
                        break;
                    case 3:
                        message[i] = policy.PolicyValueOne;
                        break;
                    case 4:
                        message[i] = policy.PolicyModuleTwo;
                        break;
                    case 5:
                        message[i] = policy.PolicyValueTwo;
                        break;
                    case 6:
                        message[i] = policy.PolicyModuleThree;
                        break;
                    case 7:
                        message[i] = policy.PolicyValueThree;
                        break;
                    case 8:
                        message[i] = policy.EmptyByte;
                        break;
                    case 9:
                        message[i] = (byte)arduinoCommunicator.messageEnd;
                        break;
                }
            }
            arduinoCommunicator.SendBytes(message);
        }

        public List<string> GetCanMessages()
        {
            return messageStorage.Messages;
        }

        public Policy GetPolicy()
        {
            //TODO: Implement
            return null;
        }

        public string GetServerName()
        {
            return "KnikkerBaanServer";
        }
    }
}
