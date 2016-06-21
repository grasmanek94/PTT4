using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dingetje
{
    public class MessageStorage
    {
        private static MessageStorage instance = new MessageStorage();

        public Policy CurrentPolicy { get; set; }

        public byte[] PolicyBytes { get; set; }

        public List<string> Messages { get; set; }

        private MessageStorage() { }

        public static MessageStorage Messagestorage
        {
            get { return instance; }
        }

        public void AddMessage(string message)
        {
            string msg = CheckForDuplicates(message);
            if(msg != null)
            {
                Messages.Remove(msg);
            }
            Messages.Add(message);
        }

        public string CheckForDuplicates(string message)
        {
            foreach (string msg in Messages)
            {
                if (message.Substring(0,4) == msg.Substring(0,4))
                {
                    return msg;
                }
            }
            return null;
        }

    }
}
