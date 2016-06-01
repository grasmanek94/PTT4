using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace KnikkerBaanServer
{
    [ServiceContract]
    public interface IScadaService
    {
        [OperationContract]
        void SetPolicy(Policy policy);

        [OperationContract]
        List<Can_Message> GetCanMessages();

        [OperationContract]
        Policy GetPolicy();

        [OperationContract]
        string GetServerName();

        // TODO: Add your service operations here
    }

    // Use a data contract as illustrated in the sample below to add composite types to service operations.
    // You can add XSD files into the project. After building the project, you can directly use the data types defined there, with the namespace "KnikkerBaanServer.ContractType".
    [DataContract]
    public class Policy
    {
       //ToDo: Determine Properties
    }

    [DataContract]
    public class Can_Message
    {
        [DataMember]
        public int Identifier { get; set; }
        [DataMember]
        public int Type { get; set; }
        [DataMember]
        public int Function { get; set; }
        [DataMember]
        public int Value { get; set; }
        [DataMember]
        public int Diagnostics { get; set; }
    }
}
