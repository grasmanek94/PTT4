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

        public List<Can_Message> Messages { get; set; }

        private MessageStorage()
        {
            Messages = new List<Can_Message>();
        }

        public static MessageStorage Messagestorage
        {
            get { return instance; }
        }

        public void AddMessage(Can_Message message)
        {
            Can_Message msg = CheckForDuplicates(message);
            if(msg != null)
            {
                Messages.Remove(msg);
            }
            Messages.Add(message);
        }

        public Can_Message CheckForDuplicates(Can_Message message)
        {
            foreach (Can_Message msg in Messages)
            {
                if (message.Identifier == msg.Identifier)
                {
                    return msg;
                }
            }
            return null;
        }

    }
}
