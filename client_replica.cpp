#include "client_replica.h"
#include "gen/stubclient.h"

#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

ClientReplica::ClientReplica(string address) {
    this->replicaAddress = address;
}

string ClientReplica::GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
    HttpClient replicaClient(this->replicaAddress);
    StubClient primaryClient(replicaClient, JSONRPC_CLIENT_V2);
    Json::Value val;
    
    try {
         val = primaryClient.GetVote(content, fhandle, filename, owner_vsID);
         return val.get("status", "").asString();
    } catch (JsonRpcException &e) {
        cerr << e.GetMessage() << endl;
    }
}