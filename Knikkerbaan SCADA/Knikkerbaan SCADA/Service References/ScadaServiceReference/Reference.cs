﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:4.0.30319.42000
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace Knikkerbaan_SCADA.ScadaServiceReference {
    using System.Runtime.Serialization;
    using System;
    
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="Policy", Namespace="http://schemas.datacontract.org/2004/07/KnikkerBaanServer")]
    [System.SerializableAttribute()]
    public partial class Policy : object, System.Runtime.Serialization.IExtensibleDataObject, System.ComponentModel.INotifyPropertyChanged {
        
        [System.NonSerializedAttribute()]
        private System.Runtime.Serialization.ExtensionDataObject extensionDataField;
        
        [global::System.ComponentModel.BrowsableAttribute(false)]
        public System.Runtime.Serialization.ExtensionDataObject ExtensionData {
            get {
                return this.extensionDataField;
            }
            set {
                this.extensionDataField = value;
            }
        }
        
        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;
        
        protected void RaisePropertyChanged(string propertyName) {
            System.ComponentModel.PropertyChangedEventHandler propertyChanged = this.PropertyChanged;
            if ((propertyChanged != null)) {
                propertyChanged(this, new System.ComponentModel.PropertyChangedEventArgs(propertyName));
            }
        }
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.Runtime.Serialization", "4.0.0.0")]
    [System.Runtime.Serialization.DataContractAttribute(Name="Can_Message", Namespace="http://schemas.datacontract.org/2004/07/KnikkerBaanServer")]
    [System.SerializableAttribute()]
    public partial class Can_Message : object, System.Runtime.Serialization.IExtensibleDataObject, System.ComponentModel.INotifyPropertyChanged {
        
        [System.NonSerializedAttribute()]
        private System.Runtime.Serialization.ExtensionDataObject extensionDataField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private int DiagnosticsField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private int FunctionField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private int IdentifierField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private int TypeField;
        
        [System.Runtime.Serialization.OptionalFieldAttribute()]
        private int ValueField;
        
        [global::System.ComponentModel.BrowsableAttribute(false)]
        public System.Runtime.Serialization.ExtensionDataObject ExtensionData {
            get {
                return this.extensionDataField;
            }
            set {
                this.extensionDataField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public int Diagnostics {
            get {
                return this.DiagnosticsField;
            }
            set {
                if ((this.DiagnosticsField.Equals(value) != true)) {
                    this.DiagnosticsField = value;
                    this.RaisePropertyChanged("Diagnostics");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public int Function {
            get {
                return this.FunctionField;
            }
            set {
                if ((this.FunctionField.Equals(value) != true)) {
                    this.FunctionField = value;
                    this.RaisePropertyChanged("Function");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public int Identifier {
            get {
                return this.IdentifierField;
            }
            set {
                if ((this.IdentifierField.Equals(value) != true)) {
                    this.IdentifierField = value;
                    this.RaisePropertyChanged("Identifier");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public int Type {
            get {
                return this.TypeField;
            }
            set {
                if ((this.TypeField.Equals(value) != true)) {
                    this.TypeField = value;
                    this.RaisePropertyChanged("Type");
                }
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public int Value {
            get {
                return this.ValueField;
            }
            set {
                if ((this.ValueField.Equals(value) != true)) {
                    this.ValueField = value;
                    this.RaisePropertyChanged("Value");
                }
            }
        }
        
        public event System.ComponentModel.PropertyChangedEventHandler PropertyChanged;
        
        protected void RaisePropertyChanged(string propertyName) {
            System.ComponentModel.PropertyChangedEventHandler propertyChanged = this.PropertyChanged;
            if ((propertyChanged != null)) {
                propertyChanged(this, new System.ComponentModel.PropertyChangedEventArgs(propertyName));
            }
        }
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    [System.ServiceModel.ServiceContractAttribute(ConfigurationName="ScadaServiceReference.IScadaService")]
    public interface IScadaService {
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IScadaService/SetPolicy", ReplyAction="http://tempuri.org/IScadaService/SetPolicyResponse")]
        void SetPolicy(Knikkerbaan_SCADA.ScadaServiceReference.Policy policy);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IScadaService/SetPolicy", ReplyAction="http://tempuri.org/IScadaService/SetPolicyResponse")]
        System.Threading.Tasks.Task SetPolicyAsync(Knikkerbaan_SCADA.ScadaServiceReference.Policy policy);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IScadaService/GetCanMessages", ReplyAction="http://tempuri.org/IScadaService/GetCanMessagesResponse")]
        Knikkerbaan_SCADA.ScadaServiceReference.Can_Message[] GetCanMessages();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IScadaService/GetCanMessages", ReplyAction="http://tempuri.org/IScadaService/GetCanMessagesResponse")]
        System.Threading.Tasks.Task<Knikkerbaan_SCADA.ScadaServiceReference.Can_Message[]> GetCanMessagesAsync();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IScadaService/GetPolicy", ReplyAction="http://tempuri.org/IScadaService/GetPolicyResponse")]
        Knikkerbaan_SCADA.ScadaServiceReference.Policy GetPolicy();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IScadaService/GetPolicy", ReplyAction="http://tempuri.org/IScadaService/GetPolicyResponse")]
        System.Threading.Tasks.Task<Knikkerbaan_SCADA.ScadaServiceReference.Policy> GetPolicyAsync();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IScadaService/GetServerName", ReplyAction="http://tempuri.org/IScadaService/GetServerNameResponse")]
        string GetServerName();
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IScadaService/GetServerName", ReplyAction="http://tempuri.org/IScadaService/GetServerNameResponse")]
        System.Threading.Tasks.Task<string> GetServerNameAsync();
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    public interface IScadaServiceChannel : Knikkerbaan_SCADA.ScadaServiceReference.IScadaService, System.ServiceModel.IClientChannel {
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("System.ServiceModel", "4.0.0.0")]
    public partial class ScadaServiceClient : System.ServiceModel.ClientBase<Knikkerbaan_SCADA.ScadaServiceReference.IScadaService>, Knikkerbaan_SCADA.ScadaServiceReference.IScadaService {
        
        public ScadaServiceClient() {
        }
        
        public ScadaServiceClient(string endpointConfigurationName) : 
                base(endpointConfigurationName) {
        }
        
        public ScadaServiceClient(string endpointConfigurationName, string remoteAddress) : 
                base(endpointConfigurationName, remoteAddress) {
        }
        
        public ScadaServiceClient(string endpointConfigurationName, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(endpointConfigurationName, remoteAddress) {
        }
        
        public ScadaServiceClient(System.ServiceModel.Channels.Binding binding, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(binding, remoteAddress) {
        }
        
        public void SetPolicy(Knikkerbaan_SCADA.ScadaServiceReference.Policy policy) {
            base.Channel.SetPolicy(policy);
        }
        
        public System.Threading.Tasks.Task SetPolicyAsync(Knikkerbaan_SCADA.ScadaServiceReference.Policy policy) {
            return base.Channel.SetPolicyAsync(policy);
        }
        
        public Knikkerbaan_SCADA.ScadaServiceReference.Can_Message[] GetCanMessages() {
            return base.Channel.GetCanMessages();
        }
        
        public System.Threading.Tasks.Task<Knikkerbaan_SCADA.ScadaServiceReference.Can_Message[]> GetCanMessagesAsync() {
            return base.Channel.GetCanMessagesAsync();
        }
        
        public Knikkerbaan_SCADA.ScadaServiceReference.Policy GetPolicy() {
            return base.Channel.GetPolicy();
        }
        
        public System.Threading.Tasks.Task<Knikkerbaan_SCADA.ScadaServiceReference.Policy> GetPolicyAsync() {
            return base.Channel.GetPolicyAsync();
        }
        
        public string GetServerName() {
            return base.Channel.GetServerName();
        }
        
        public System.Threading.Tasks.Task<string> GetServerNameAsync() {
            return base.Channel.GetServerNameAsync();
        }
    }
}