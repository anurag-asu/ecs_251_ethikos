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
      Json::Value result = c.FileLookUp("123", "dummy", "01");

      // get votes from all replicas
      ClientReplica c1{result.get("host_urls","")[0].asString()};
      string vote1 = c1.GetVote(1, "","","");
      cout<<vote1<<endl;

      ClientReplica c2{result.get("host_urls","")[1].asString()};
      string vote2 = c2.GetVote(1, "","","");
      cout<<vote2<<endl;

      ClientReplica c3{result.get("host_urls","")[2].asString()};
      string vote3 = c3.GetVote(1, "","","");
      cout<<vote3<<endl;

      // send commit/abort to all replicas

      if(vote1 == "commit" && vote1 == vote2 && vote1 == vote3) {
        bool s1 = c1.CommitOrAbort("commit", 1, "", "", "");
        bool s2 = c1.CommitOrAbort("commit", 1, "", "", "");
        bool s3 = c1.CommitOrAbort("commit", 1, "", "", "");

        if(s1 && s2 && s3) {
          cout<<"success"<<endl;
        } else {
          cout<<"failure"<<endl;
        }

      } else {
        bool s1 = c1.CommitOrAbort("abort", 1, "", "", "");
        bool s2 = c1.CommitOrAbort("abort", 1, "", "", "");
        bool s3 = c1.CommitOrAbort("abort", 1, "", "", "");

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