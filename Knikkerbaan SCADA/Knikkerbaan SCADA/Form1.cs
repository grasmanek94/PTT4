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
    public enum NodeNames : byte { NodePC=00000001, Node1=00000010, Node2=00000011, Node3=00000100,Node4=00000101,Node5=00000110 };
    public enum Node1Actions : byte { Hoog=00000001, Laag=00000010, Alle=00000011, Geen=00000100 };
    public enum Node2Actions : byte { Transparant=00000001, Opaque=00000010, Alle=00000011, Geen=00000100 };
    public enum Node3Actions : byte { Wit=00000001, Groen=00000010, Zwart=00000011, Alle=00000100, Geen=00000101 };

    public partial class Form1 : Form
    {
        ScadaServiceClient proxy;
        List<string> Messages;
        public Form1()
        {
            InitializeComponent();
            proxy = new ScadaServiceClient();
            FillComboboxes();
            Messages = new List<string>();
        }

        
        private void ConnectToServerButton_Click(object sender, EventArgs e)
        {
            ServerNameTextbox.Text = proxy.GetServerName();
        }

        private void SetPolicyButton_Click(object sender, EventArgs e)
        {
            Policy policy = new Policy();
            policy.PolicyModuleOne = (byte)NodeNames.Node1;
            policy.PolicyValueOne = (byte)PolicyNode1Combobox.SelectedItem;
            policy.PolicyModuleTwo = (byte)NodeNames.Node2;
            policy.PolicyValueTwo = (byte)PolicyNode2Combobox.SelectedItem;
            policy.PolicyModuleThree = (byte)NodeNames.Node3;
            policy.PolicyValueThree = (byte)PolicyNode3Combobox.SelectedItem;
            proxy.SetPolicy(policy);
        }

        private void FillComboboxes()
        {
            PolicyNode1Combobox.DataSource = Enum.GetNames(typeof(Node1Actions));
            PolicyNode2Combobox.DataSource = Enum.GetNames(typeof(Node2Actions));
            PolicyNode3Combobox.DataSource = Enum.GetNames(typeof(Node3Actions));
        }

        private void ReceiveData()
        {
            string[] messages = proxy.GetCanMessages();
            foreach(string msg in messages)
            {
                Messages.Add(msg);
            }
        }

        private void ShowData()
        {
            foreach(string msg in Messages)
            {
                if(msg.Substring(0,8) == "00000010")
                {
                    Node1Info.Text = msg.Substring(24, 8);
                }
                if (msg.Substring(0, 8) == "000000011")
                {
                    Node2Info.Text = msg.Substring(24, 8);
                }
                if (msg.Substring(0, 8) == "000000100")
                {
                    Node3Info.Text = msg.Substring(24, 8);
                }
            }
        }
    }
}
