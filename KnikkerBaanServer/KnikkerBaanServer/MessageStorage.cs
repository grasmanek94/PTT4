using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KnikkerBaanServer
{
    public class MessageStorage
    {
        private static MessageStorage instance = new MessageStorage();

        public Policy CurrentPolicy { get; set; }

        public List<CanMessage> Messages { get; set; }

        private MessageStorage() { }

        public static MessageStorage Messagestorage
        {
            get { return instance; }
        }

        public void AddMessage(CanMessage message)
        {
            CanMessage msg = CheckForDuplicates(message);
            if(msg != null)
            {
                Messages.Remove(msg);
            }
            Messages.Add(message);
        }

        public CanMessage CheckForDuplicates(CanMessage message)
        {
            foreach (CanMessage msg in Messages)
            {
                if (message.MessageBytes[0] == msg.MessageBytes[0])
                {
                    return msg;
                }
            }
            return null;
        }

    }
}
