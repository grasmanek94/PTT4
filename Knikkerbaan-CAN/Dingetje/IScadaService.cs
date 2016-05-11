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
       //ToDo: Determine Properties
    }
}
