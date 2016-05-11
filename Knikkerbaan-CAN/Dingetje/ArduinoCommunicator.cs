using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO.Ports;
using System.IO;

namespace Dingetje
{
    class ArduinoCommunicator
    {
        public event Action<string> MessageFound;

        private const int bufferSize = 32;
        private const string messageStart = ">";
        private const string messageEnd = ";";

        private SerialPort serialPort;
        private string buffer;
        private Thread receiveThread;
        private bool startReceiving;

        public ArduinoCommunicator(string portName, int baudRate)
        {
            serialPort = new SerialPort(portName, baudRate);
            serialPort.Encoding = Encoding.ASCII;
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
            buffer = "";
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
                    while (serialPort.BytesToRead > 0)
                    {
                        /*byte[] bytes = new byte[serialPort.BytesToRead];
                        serialPort.Read(bytes, 0, serialPort.BytesToRead);

                        ASCIIEncoding encoder = new ASCIIEncoding();
                        string message = encoder.GetString(bytes);*/
                        string message = serialPort.ReadExisting();
                        buffer += message;

                        string found = FindMessages();
                        if (found != null &&
                            MessageFound != null)
                        {
                            MessageFound(found);
                        }
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

        private string FindMessages()
        {
            int start = buffer.IndexOf(messageStart);
            
            if (start != -1)
            {
                int end = buffer.IndexOf(messageEnd, start);
                if (end != -1)
                {
                    string msg = buffer.Substring(
                    start, (end - start) + 1);
                    buffer = buffer.Substring(end + 1);

                    return msg;
                }
                
            }

            return null;
        }
    }
}
