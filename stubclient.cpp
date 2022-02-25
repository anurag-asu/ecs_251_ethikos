#include <iostream>

#include "gen/stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "client_primary_replica.h"

using namespace jsonrpc;
using namespace std;

int main() {
  HttpClient httpclient("http://localhost:8383");
  // StubClient c(httpclient, JSONRPC_CLIENT_V1); //json-rpc 1.0
  StubClient c(httpclient, JSONRPC_CLIENT_V2); // json-rpc 2.0

  try {
      Json::Value result =  c.FileLookUp("123", "dummy", "01");
      cout<<result.toStyledString();

      // Json::Value result1 = c.CommitOrAbort(1, "", "", "");
      // Json::Value result2 = c.GetVote(1, "", "", "");
      
      // ClientPrimaryReplica c1{s};
      // c1.ConnectToPrimaryReplica();
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
}