using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace KnikkerBaanServer
{
    public class CanMessage
    {
        public Byte[] MessageBytes { get; set; }

        public CanMessage()
        {
            MessageBytes = new Byte[8];
        }
    }
}
