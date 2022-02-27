#include <iostream>

#include "gen/stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "client_replica.h"

using namespace jsonrpc;
using namespace std;

int main() {
  HttpClient httpclient("http://localhost:8383");
  // StubClient c(httpclient, JSONRPC_CLIENT_V1); //json-rpc 1.0
  StubClient c(httpclient, JSONRPC_CLIENT_V2); // json-rpc 2.0

  try {

      // get a list of all replica server addresses
      Json::Value result = c.FileLookUp(2, "123", "dummy", "01");

      // get votes from all replicas
      ClientReplica c4{result.get("host_urls","")[0].asString()};
      string vote1 = c4.GetVote("test", "123", "dummy", 1,  "01");
      cout<<vote1<<endl;

      ClientReplica c5{result.get("host_urls","")[1].asString()};
      string vote2 = c5.GetVote("test", "123", "dummy", 1,  "01");
      cout<<vote2<<endl;

      ClientReplica c6{result.get("host_urls","")[2].asString()};
      string vote3 = c6.GetVote("test", "123", "dummy", 1,  "01");
      cout<<vote3<<endl;

      // send commit/abort to all replicas

      if(vote1 == "commit" && vote1 == vote2 && vote1 == vote3) {
        bool s1 = c4.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");
        bool s2 = c5.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");
        bool s3 = c6.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");

        if(s1 && s2 && s3) {
          cout<<"success"<<endl;
        } else {
          cout<<"failure"<<endl;
        }

      } else {
        bool s1 = c4.CommitOrAbort("abort", "test", "123", "dummy", 1,  "01");
        bool s2 = c5.CommitOrAbort("abort", "test", "123", "dummy", 1,  "01");
        bool s3 = c6.CommitOrAbort("abort", "test", "123", "dummy", 1,  "01");

        if(s1 && s2 && s3) {
          cout<<"success"<<endl;
        } else {
          cout<<"failure"<<endl;
        }
      }

  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
}