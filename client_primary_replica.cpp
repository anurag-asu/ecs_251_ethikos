#include "client_primary_replica.h"
#include "gen/stubclient.h"

#include <jsonrpccpp/client/connectors/httpclient.h>

using namespace jsonrpc;
using namespace std;

ClientPrimaryReplica::ClientPrimaryReplica(string address) {
    this->replicaAddress = address;
}

void ClientPrimaryReplica::ConnectToPrimaryReplica() {
    HttpClient httpclient((this->replicaAddress).c_str());
    StubClient myClient(httpclient, JSONRPC_CLIENT_V2);

    try {
        myClient.connectToPrimaryReplica();
    } catch (JsonRpcException &e) {
        cerr << e.what() << endl;
    }
}