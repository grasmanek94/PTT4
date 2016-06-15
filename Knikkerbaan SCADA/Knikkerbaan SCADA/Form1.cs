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
    public enum NodeNames : byte { NodePC=0x01, Node1=0x02, Node2=0x03, Node3=0x04,Node4=0x05,Node5=0x06 };
    public enum Node1Actions : byte { Hoog=0x01, Laag=0x02, Alle=0x03, Geen=0x04 };
    public enum Node2Actions : byte { Transparant=0x01, Opaque=0x02, Alle=0x03, Geen=0x04 };
    public enum Node3Actions : byte { Wit=0x01, Groen=0x02, Zwart=0x03, Alle=0x04, Geen=0x05 };

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
            policy.PolicyValueOne = lookupPolicy(NodeNames.Node1, PolicyNode1Combobox.SelectedItem.ToString());
            policy.PolicyModuleTwo = (byte)NodeNames.Node2;
            policy.PolicyValueTwo = lookupPolicy(NodeNames.Node2, PolicyNode2Combobox.SelectedItem.ToString());
            policy.PolicyModuleThree = (byte)NodeNames.Node3;
            policy.PolicyValueThree = lookupPolicy(NodeNames.Node3, PolicyNode3Combobox.SelectedItem.ToString());
            proxy.SetPolicy(policy);
        }

        // This can probably be more efficient but eh design choices
        private byte lookupPolicy(NodeNames node, string item)
        {
            switch (node)
            {
                case NodeNames.Node1:
                    switch(item)
                    {
                        case "Hoog":
                            return (byte)Node1Actions.Hoog;
                        case "Laag":
                            return (byte)Node1Actions.Laag;
                        case "Alle":
                            return (byte)Node1Actions.Alle;
                        case "Geen":
                            return (byte)Node1Actions.Geen;
                    }
                    break;
                case NodeNames.Node2:
                    switch (item)
                    {
                        case "Transparant":
                            return (byte)Node2Actions.Transparant;
                        case "Opaque":
                            return (byte)Node2Actions.Opaque;
                        case "Alle":
                            return (byte)Node2Actions.Alle;
                        case "Geen":
                            return (byte)Node2Actions.Geen;
                    }
                    break;
                case NodeNames.Node3:
                    switch (item)
                    {
                        case "Wit":
                            return (byte)Node3Actions.Wit;
                        case "Groen":
                            return (byte)Node3Actions.Groen;
                        case "Zwart":
                            return (byte)Node3Actions.Zwart;
                        case "Alle":
                            return (byte)Node3Actions.Alle;
                        case "Geen":
                            return (byte)Node3Actions.Geen;
                    }
                    break;
            }
            return 0xFF; // something went awfully wrong
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
