using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.Ports;
using System.IO;

namespace KnikkerBaanServer
{
    public class ArduinoCommunicator
    {
        public event Action<Can_Message> MessageFound;

        private const int bufferSize = 32;
        private const string messageStart = ">";
        private const string messageEnd = ";";

        private SerialPort serialPort;
        private Byte[] buffer;
        private Thread receiveThread;
        private bool startReceiving;
        MessageStorage messageStorage = MessageStorage.Messagestorage;

        public ArduinoCommunicator(string portName, int baudRate)
        {
            serialPort = new SerialPort(portName, baudRate);
            serialPort.Encoding = Encoding.ASCII;
            buffer = new Byte[8];
        }

        public bool SendBytes(byte[] message)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Write(message, 0, message.Length);
                return true;
            }
            return false;
        }

        public bool SendMessage(string message)
        {
            if (serialPort.IsOpen)
            {
                serialPort.Write(message);
                return true;
            }
            return false;
        }

        public void Start()
        {
            buffer[0] = 0;
            startReceiving = true;
            receiveThread = new Thread(new ThreadStart(Receive));
            receiveThread.Start();
        }

        private void Receive()
        {
            serialPort.Open();

            EventWaitHandle waithandler = new EventWaitHandle(
                false,
                EventResetMode.AutoReset,
                Guid.NewGuid().ToString());

            while (serialPort.IsOpen &&
                   startReceiving)
            {
                try
                {
                    int i = 0;
                    while (serialPort.BytesToRead > 0)
                    {
                        /*byte[] bytes = new byte[serialPort.BytesToRead];
                        serialPort.Read(bytes, 0, serialPort.BytesToRead);

                        ASCIIEncoding encoder = new ASCIIEncoding();
                        string message = encoder.GetString(bytes);*/
                        //Byte message = serialPort.ReadExisting();
                        
                        Byte Message = (Byte)serialPort.ReadByte();

                        if (i < 8)
                        {
                            buffer[i] = Message;

                            
                            i++;
                        }
                    }
                    Can_Message message = FindMessage(buffer);
                    if (message != null &&
                    MessageFound != null)
                    {
                        messageStorage.AddMessage(message);
                        MessageFound(message);
                    }
                }
                catch (IOException)
                {
                    break;
                }


                waithandler.WaitOne(1);
            }

            serialPort.Close();
        }

        private Can_Message FindMessage(Byte[] message)
        {
            Can_Message msg = new Can_Message();

            msg.Identifier = message[0];
            msg.Type = message[1];
            msg.Function = message[2];
            msg.Value = message[3];
            msg.Diagnostics = message[4];
            return msg;
        }
    }
}
