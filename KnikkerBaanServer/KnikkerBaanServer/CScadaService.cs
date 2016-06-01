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
        string ServerName = "The KnikkerBaanServer";
        MessageStorage messageStorage = MessageStorage.Messagestorage;

        public CScadaService()
        {
            Can_Message canMessage = new Can_Message();
            canMessage.Identifier = 1;
            canMessage.Type = 3;
            canMessage.Function = 2;
            canMessage.Value = 118;
            canMessage.Diagnostics = 150;
            messageStorage.AddMessage(canMessage);
        }

        public void SetPolicy(Policy policy)
        {
            //TODO: Implement
        }

        public List<Can_Message> GetCanMessages()
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
            return ServerName;
        }
    }
}
