using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Dingetje
{
    public partial class Form1 : Form
    {
        MessageStorage msgStor;
        public Form1()
        {
            InitializeComponent();
            ArduinoCommunicator ard = new ArduinoCommunicator("COM7", 9600);
            ard.MessageFound += Ard_MessageFound;
            ard.Start();
            msgStor = MessageStorage.Messagestorage;
        }

        private void Ard_MessageFound(string obj)
        {
            if (richTextBox1.InvokeRequired)
            {
                Invoke(new Action(() =>
                {
                    // richTextBox1.Text = obj;
                    richTextBox1.AppendText(obj + "\n");
                    
                    richTextBox1.ScrollToCaret();
                    int start = obj.IndexOf(":") + 2;
                    int end = obj.IndexOf(";");
                    int num;
                    Int32.TryParse(obj.Substring(start, end - start), out num);
                    messageBox.Text = num.ToString();
                    msgStor.AddMessage(obj);

                    if (obj.StartsWith(">0xFF"))
                    {
                        progressBar1.Value = num;
                    }
                    if (obj.StartsWith(">0xDD"))
                    {
                        progressBar2.Value = num;
                    }
                }));
            }
        }
    }
}
