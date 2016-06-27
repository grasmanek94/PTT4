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
        static Policy policy_;

        ArduinoCommunicator arduinoCommunicator = new ArduinoCommunicator("COM4", 9600);
        MessageStorage messageStorage = MessageStorage.Messagestorage;
        public void SetPolicy(Policy policy)
        {
            byte[] message = new byte[10];
        
            message[0] = (byte)arduinoCommunicator.messageStart;
            message[1] = policy.senderAddress;
            message[2] = policy.receiverAddress;
            message[3] = policy.PolicyModuleOne;
            message[4] = policy.PolicyValueOne;
            message[5] = policy.PolicyModuleTwo;
            message[6] = policy.PolicyValueTwo;
            message[7] = policy.PolicyModuleThree;
            message[8] = policy.PolicyValueThree;
            message[9] = (byte)arduinoCommunicator.messageEnd;

            arduinoCommunicator.SendBytes(message);

            policy_ = policy;
        }

        public List<string> GetCanMessages()
        {
            return messageStorage.Messages;
        }

        public Policy GetPolicy()
        {
            return policy_;
        }

        public string GetServerName()
        {
            return "KnikkerBaanServer";
        }
    }
}
