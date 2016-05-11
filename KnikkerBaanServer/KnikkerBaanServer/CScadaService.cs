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
        MessageStorage messageStorage = MessageStorage.Messagestorage;
        public void SetPolicy(Policy policy)
        {
            //TODO: Implement
        }

        public List<CanMessage> GetCanMessages()
        {
            return messageStorage.Messages;
        }

        public Policy GetPolicy()
        {
            //TODO: Implement
            return null;
        }
    }
}
