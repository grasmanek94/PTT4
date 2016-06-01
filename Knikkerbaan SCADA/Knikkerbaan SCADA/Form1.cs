using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.ServiceModel;
using Knikkerbaan_SCADA.ScadaServiceReference;

namespace Knikkerbaan_SCADA
{
    public partial class Form1 : Form
    {
        ScadaServiceClient proxy; 

        public Form1()
        {
            InitializeComponent();
            proxy = new ScadaServiceClient();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            string ServerName = proxy.GetServerName();
            textBox1.Text = ServerName;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Can_Message[] Messages = proxy.GetCanMessages();
            foreach(Can_Message message in Messages)
            {
                string msg = "ID:" + message.Identifier + ",TYPE:" + message.Type + ",FUNCTION:" + message.Function + ",VALUE:" + message.Value + ",DIAG:" + message.Diagnostics; 
                listBox1.Items.Add(msg);
            }
        }
    }
}
