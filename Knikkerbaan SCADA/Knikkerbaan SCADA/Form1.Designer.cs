namespace Knikkerbaan_SCADA
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.ServerNameTextbox = new System.Windows.Forms.TextBox();
            this.PolicyNode1Combobox = new System.Windows.Forms.ComboBox();
            this.PolicyNode2Combobox = new System.Windows.Forms.ComboBox();
            this.PolicyNode3Combobox = new System.Windows.Forms.ComboBox();
            this.SetPolicyButton = new System.Windows.Forms.Button();
            this.ConnectToServerButton = new System.Windows.Forms.Button();
            this.Node1Label = new System.Windows.Forms.Label();
            this.Node2Label = new System.Windows.Forms.Label();
            this.Node3Label = new System.Windows.Forms.Label();
            this.Node1Info = new System.Windows.Forms.TextBox();
            this.Node2Info = new System.Windows.Forms.TextBox();
            this.Node3Info = new System.Windows.Forms.TextBox();
            this.tbRawMsg = new System.Windows.Forms.TextBox();
            this.btnRawMsg = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // ServerNameTextbox
            // 
            this.ServerNameTextbox.Location = new System.Drawing.Point(13, 13);
            this.ServerNameTextbox.Name = "ServerNameTextbox";
            this.ServerNameTextbox.Size = new System.Drawing.Size(100, 20);
            this.ServerNameTextbox.TabIndex = 0;
            // 
            // PolicyNode1Combobox
            // 
            this.PolicyNode1Combobox.FormattingEnabled = true;
            this.PolicyNode1Combobox.Location = new System.Drawing.Point(13, 171);
            this.PolicyNode1Combobox.Name = "PolicyNode1Combobox";
            this.PolicyNode1Combobox.Size = new System.Drawing.Size(121, 21);
            this.PolicyNode1Combobox.TabIndex = 1;
            // 
            // PolicyNode2Combobox
            // 
            this.PolicyNode2Combobox.FormattingEnabled = true;
            this.PolicyNode2Combobox.Location = new System.Drawing.Point(230, 171);
            this.PolicyNode2Combobox.Name = "PolicyNode2Combobox";
            this.PolicyNode2Combobox.Size = new System.Drawing.Size(121, 21);
            this.PolicyNode2Combobox.TabIndex = 2;
            // 
            // PolicyNode3Combobox
            // 
            this.PolicyNode3Combobox.FormattingEnabled = true;
            this.PolicyNode3Combobox.Location = new System.Drawing.Point(435, 171);
            this.PolicyNode3Combobox.Name = "PolicyNode3Combobox";
            this.PolicyNode3Combobox.Size = new System.Drawing.Size(121, 21);
            this.PolicyNode3Combobox.TabIndex = 3;
            // 
            // SetPolicyButton
            // 
            this.SetPolicyButton.Location = new System.Drawing.Point(602, 169);
            this.SetPolicyButton.Name = "SetPolicyButton";
            this.SetPolicyButton.Size = new System.Drawing.Size(75, 23);
            this.SetPolicyButton.TabIndex = 4;
            this.SetPolicyButton.Text = "Set Policy";
            this.SetPolicyButton.UseVisualStyleBackColor = true;
            this.SetPolicyButton.Click += new System.EventHandler(this.SetPolicyButton_Click);
            // 
            // ConnectToServerButton
            // 
            this.ConnectToServerButton.Location = new System.Drawing.Point(119, 11);
            this.ConnectToServerButton.Name = "ConnectToServerButton";
            this.ConnectToServerButton.Size = new System.Drawing.Size(75, 23);
            this.ConnectToServerButton.TabIndex = 5;
            this.ConnectToServerButton.Text = "Connect";
            this.ConnectToServerButton.UseVisualStyleBackColor = true;
            this.ConnectToServerButton.Click += new System.EventHandler(this.ConnectToServerButton_Click);
            // 
            // Node1Label
            // 
            this.Node1Label.AutoSize = true;
            this.Node1Label.Location = new System.Drawing.Point(10, 69);
            this.Node1Label.Name = "Node1Label";
            this.Node1Label.Size = new System.Drawing.Size(42, 13);
            this.Node1Label.TabIndex = 6;
            this.Node1Label.Text = "Node 1";
            // 
            // Node2Label
            // 
            this.Node2Label.AutoSize = true;
            this.Node2Label.Location = new System.Drawing.Point(227, 69);
            this.Node2Label.Name = "Node2Label";
            this.Node2Label.Size = new System.Drawing.Size(42, 13);
            this.Node2Label.TabIndex = 7;
            this.Node2Label.Text = "Node 2";
            // 
            // Node3Label
            // 
            this.Node3Label.AutoSize = true;
            this.Node3Label.Location = new System.Drawing.Point(432, 69);
            this.Node3Label.Name = "Node3Label";
            this.Node3Label.Size = new System.Drawing.Size(42, 13);
            this.Node3Label.TabIndex = 8;
            this.Node3Label.Text = "Node 3";
            // 
            // Node1Info
            // 
            this.Node1Info.Location = new System.Drawing.Point(12, 145);
            this.Node1Info.Name = "Node1Info";
            this.Node1Info.Size = new System.Drawing.Size(121, 20);
            this.Node1Info.TabIndex = 9;
            // 
            // Node2Info
            // 
            this.Node2Info.Location = new System.Drawing.Point(230, 145);
            this.Node2Info.Name = "Node2Info";
            this.Node2Info.Size = new System.Drawing.Size(121, 20);
            this.Node2Info.TabIndex = 10;
            // 
            // Node3Info
            // 
            this.Node3Info.Location = new System.Drawing.Point(435, 145);
            this.Node3Info.Name = "Node3Info";
            this.Node3Info.Size = new System.Drawing.Size(121, 20);
            this.Node3Info.TabIndex = 11;
            // 
            // tbRawMsg
            // 
            this.tbRawMsg.Location = new System.Drawing.Point(13, 231);
            this.tbRawMsg.Name = "tbRawMsg";
            this.tbRawMsg.Size = new System.Drawing.Size(211, 20);
            this.tbRawMsg.TabIndex = 12;
            // 
            // btnRawMsg
            // 
            this.btnRawMsg.Location = new System.Drawing.Point(230, 229);
            this.btnRawMsg.Name = "btnRawMsg";
            this.btnRawMsg.Size = new System.Drawing.Size(75, 23);
            this.btnRawMsg.TabIndex = 13;
            this.btnRawMsg.Text = "btnRawMsg";
            this.btnRawMsg.UseVisualStyleBackColor = true;
            this.btnRawMsg.Click += new System.EventHandler(this.btnRawMsg_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 212);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(343, 13);
            this.label1.TabIndex = 14;
            this.label1.Text = "Enter hex values separated by commas, eg: 0x01,0x02, for 8 bytes kthx";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(689, 261);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnRawMsg);
            this.Controls.Add(this.tbRawMsg);
            this.Controls.Add(this.Node3Info);
            this.Controls.Add(this.Node2Info);
            this.Controls.Add(this.Node1Info);
            this.Controls.Add(this.Node3Label);
            this.Controls.Add(this.Node2Label);
            this.Controls.Add(this.Node1Label);
            this.Controls.Add(this.ConnectToServerButton);
            this.Controls.Add(this.SetPolicyButton);
            this.Controls.Add(this.PolicyNode3Combobox);
            this.Controls.Add(this.PolicyNode2Combobox);
            this.Controls.Add(this.PolicyNode1Combobox);
            this.Controls.Add(this.ServerNameTextbox);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox ServerNameTextbox;
        private System.Windows.Forms.ComboBox PolicyNode1Combobox;
        private System.Windows.Forms.ComboBox PolicyNode2Combobox;
        private System.Windows.Forms.ComboBox PolicyNode3Combobox;
        private System.Windows.Forms.Button SetPolicyButton;
        private System.Windows.Forms.Button ConnectToServerButton;
        private System.Windows.Forms.Label Node1Label;
        private System.Windows.Forms.Label Node2Label;
        private System.Windows.Forms.Label Node3Label;
        private System.Windows.Forms.TextBox Node1Info;
        private System.Windows.Forms.TextBox Node2Info;
        private System.Windows.Forms.TextBox Node3Info;
        private System.Windows.Forms.TextBox tbRawMsg;
        private System.Windows.Forms.Button btnRawMsg;
        private System.Windows.Forms.Label label1;
    }
}

