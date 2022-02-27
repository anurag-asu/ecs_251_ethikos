#include <iostream>

#include "gen/stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>

#include "client_replica.h"

using namespace jsonrpc;
using namespace std;

HttpClient httpclient("http://localhost:8383");
  // StubClient c(httpclient, JSONRPC_CLIENT_V1); //json-rpc 1.0
StubClient c(httpclient, JSONRPC_CLIENT_V2); // json-rpc 2.0

void clientFirstChunk() {
   // get a list of all replica server addresses
    Json::Value addressChunk1 = c.FileLookUp(1, "123", "dummy", "01");

    // get votes from all replicas
    ClientReplica r1{addressChunk1.get("host_urls","")[0].asString()};
    r1.ShowFileContents();

    ClientReplica r2{addressChunk1.get("host_urls","")[1].asString()};
    r2.ShowFileContents();

    ClientReplica r3{addressChunk1.get("host_urls","")[2].asString()};
    r3.ShowFileContents();

    string vote1 = r1.GetVote("test", "123", "dummy", 1,  "01");  
    string vote2 = r2.GetVote("test", "123", "dummy", 1,  "01");
    string vote3 = r3.GetVote("test", "123", "dummy", 1,  "01");

    // send commit/abort to all replicas
    if(vote1 == "commit" && vote1 == vote2 && vote1 == vote3) {
      bool s1 = r1.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");
      bool s2 = r2.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");
      bool s3 = r3.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");

      r1.ShowFileContents();
      r2.ShowFileContents();
      r3.ShowFileContents();

      if(s1 && s2 && s3) {
        cout<<"success"<<endl;
      } else {
        cout<<"failure"<<endl;
      }

    } else {
      bool s1 = r1.CommitOrAbort("abort", "test", "123", "dummy", 1,  "01");
      bool s2 = r2.CommitOrAbort("abort", "test", "123", "dummy", 1,  "01");
      bool s3 = r3.CommitOrAbort("abort", "test", "123", "dummy", 1,  "01");

      if(s1 && s2 && s3) {
        cout<<"success"<<endl;
      } else {
        cout<<"failure"<<endl;
      }
    }
}

void clientFailedInconsistency() {
  Json::Value addressChunk1 = c.FileLookUp(1, "123", "dummy", "01");
  Json::Value addressChunk2 = c.FileLookUp(2, "123", "dummy", "01");

  ClientReplica r1{addressChunk1.get("host_urls","")[0].asString()};
  r1.ShowFileContents();

  ClientReplica r2{addressChunk1.get("host_urls","")[1].asString()};
  r2.ShowFileContents();

  ClientReplica r3{addressChunk1.get("host_urls","")[2].asString()};
  r3.ShowFileContents();

  string vote1 = r1.GetVote("test", "123", "dummy", 1,  "01");  
  string vote2 = r2.GetVote("test", "123", "dummy", 1,  "01");
  string vote3 = r3.GetVote("test", "123", "dummy", 1,  "01");
  
  // send commit/abort to all replicas
  if(vote1 == "commit" && vote1 == vote2 && vote1 == vote3) {
    bool s1 = r1.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");
    bool s2 = r2.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");

    // assume network or replica failed and we cannot commit
    // bool s3 = r3.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");

    cout<<"Please check the data in replica servers for incosistency"<<endl;
    r1.ShowFileContents();
    r2.ShowFileContents();
    r3.ShowFileContents();

  }

}

int main() {
  try {
    // clientFirstChunk();
    clientFailedInconsistency();
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
}
