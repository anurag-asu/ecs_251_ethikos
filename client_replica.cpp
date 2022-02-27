#include "client_replica.h"
#include "gen/stubclient.h"

#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

ClientReplica::ClientReplica(string address) {
    this->replicaAddress = address;
}

string ClientReplica::GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID) {
    HttpClient replicaClient(this->replicaAddress);
    StubClient primaryClient(replicaClient, JSONRPC_CLIENT_V2);
    Json::Value val;
    
    try {
         val = primaryClient.GetVote(content, fhandle, filename, offset, owner_vsID);
         return val.get("status", "").asString();
    } catch (JsonRpcException &e) {
        cerr << e.GetMessage() << endl;
    }
}

bool ClientReplica::CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID) {
    HttpClient replicaClient(this->replicaAddress);
    StubClient primaryClient(replicaClient, JSONRPC_CLIENT_V2);
    Json::Value val;
    
    try {
         val = primaryClient.CommitOrAbort(action, content, fhandle, filename, offset, owner_vsID);
         return val.get("status", false).asBool();
    } catch (JsonRpcException &e) {
        cerr << e.GetMessage() << endl;
    }
}