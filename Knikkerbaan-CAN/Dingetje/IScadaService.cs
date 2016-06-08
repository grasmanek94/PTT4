using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;

namespace Dingetje
{
    [ServiceContract]
    public interface IScadaService
    {
        [OperationContract]
        void SetPolicy(Policy policy);

        [OperationContract]
        List<string> GetCanMessages();

        [OperationContract]
        Policy GetPolicy();

        // TODO: Add your service operations here
    }

    // Use a data contract as illustrated in the sample below to add composite types to service operations.
    // You can add XSD files into the project. After building the project, you can directly use the data types defined there, with the namespace "KnikkerBaanServer.ContractType".
    [DataContract]
    public class Policy
    {
        [DataMember]
        public byte address { get; set; }
        [DataMember]
        public byte PolicyModuleOne { get; set; }
        [DataMember]
        public byte PolicyValueOne { get; set; }
        [DataMember]
        public byte PolicyModuleTwo { get; set; }
        [DataMember]
        public byte PolicyValueTwo { get; set; }
        [DataMember]
        public byte PolicyModuleThree { get; set; }
        [DataMember]
        public byte PolicyValueThree { get; set; }
        [DataMember]
        public byte EmptyByte { get; private set; }
    }
}
