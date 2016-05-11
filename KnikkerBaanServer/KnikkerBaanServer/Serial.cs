using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO.Ports;
using System.IO;

namespace KnikkerBaanServer
{
    public class Serial
    {
        public MessageStorage messageStorage;
        public string PortName { get; set; }
        public int BaudRate { get; set; }
        public SerialPort sPort { get; private set; }

        public Serial(string portName, int baudRate)
        {
            if (portName == null || portName == "")
            {
                throw new ArgumentNullException();
            }
            if (baudRate <= 0)
            {
                throw new ArgumentOutOfRangeException();
            }
            PortName = portName;
            BaudRate = baudRate;
            sPort = new SerialPort(PortName, BaudRate);
            messageStorage = MessageStorage.Messagestorage;
        }

        public bool ReadMessage()
        {
            CanMessage canMessage = new CanMessage();
            int readbytes = 0;
            if (sPort.IsOpen)
            {
                for(int i = 0; i < 8; i++)
                {
                    readbytes = sPort.Read(canMessage.MessageBytes, 0, 8);
                }
                if(readbytes == 8)
                {
                    messageStorage.AddMessage(canMessage);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        public bool OpenPort()
        {
            sPort.Open();
            if(sPort.IsOpen)
            {
                return true;
            }
            return false;
        }

        public bool ClosePort()
        {
            sPort.Close();
            if(!sPort.IsOpen)
            {
                return true;
            }
            return false;
        }
    }
}
