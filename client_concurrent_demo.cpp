#include <iostream>
#include "gen/stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include <thread>
#include "client_replica.h"

using namespace jsonrpc;
using namespace std;

HttpClient httpclient("http://localhost:8383");
  // StubClient c(httpclient, JSONRPC_CLIENT_V1); //json-rpc 1.0
StubClient c(httpclient, JSONRPC_CLIENT_V2); // json-rpc 2.0

HttpClient httpclient2("http://localhost:8282");
StubClient d(httpclient, JSONRPC_CLIENT_V2);
void thread() {

}
void execute(ClientReplica r1,ClientReplica r2,ClientReplica r3,int clientNum){
  string vote1 = r1.GetVote("test", "123", "dummy", 1,  "01");  
  string vote2 = r2.GetVote("test", "123", "dummy", 1,  "01");
  string vote3 = r3.GetVote("test", "123", "dummy", 1,  "01");
  if(vote1 == "commit" && vote1 == vote2 && vote1 == vote3) {
      bool s1 = r1.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");
      bool s2 = r2.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");
      bool s3 = r3.CommitOrAbort("commit", "test", "123", "dummy", 1,  "01");
    r1.ShowFileContents();
    r2.ShowFileContents();
    r3.ShowFileContents();
    if(s1 && s2 && s3) {
      cout<<"client " << clientNum << " success"<<endl;
      } else {
      cout<<"client " << clientNum << " failure"<<endl;
    }
  }
}
void bothclientconcFirstChunk() {
    //first client
   // get a list of all replica server addresses
    Json::Value addressChunk1 = c.FileLookUp(1, "123", "dummy", "01");

    // get votes from all replicas
    ClientReplica r1{addressChunk1.get("host_urls","")[0].asString()};
    r1.ShowFileContents();

    ClientReplica r2{addressChunk1.get("host_urls","")[1].asString()};
    r2.ShowFileContents();

    ClientReplica r3{addressChunk1.get("host_urls","")[2].asString()};
    r3.ShowFileContents();

    Json::Value addressChunkrc2_1 = d.FileLookUp(1, "123", "dummy", "01");

    // get votes from all replicas
    ClientReplica rc2_1{addressChunkrc2_1.get("host_urls","")[0].asString()};
    rc2_1.ShowFileContents();

    ClientReplica rc2_2{addressChunkrc2_1.get("host_urls","")[1].asString()};
    rc2_2.ShowFileContents();

    ClientReplica rc2_3{addressChunkrc2_1.get("host_urls","")[2].asString()};
    rc2_3.ShowFileContents();
    vector<std::thread> threads;

    threads.push_back(std::thread(execute,r1,r2,r3,1));
    threads.push_back(std::thread(execute,rc2_1,rc2_2,rc2_3,2));

    for (auto& t: threads)
      t.join();
    /*
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
        cout<<"client 1 success"<<endl;
      } else {
        cout<<"client 1 failure"<<endl;
      }

    } else {
      bool s1 = r1.CommitOrAbort("abort", "test", "123", "dummy", 1,  "01");
      bool s2 = r2.CommitOrAbort("abort", "test", "123", "dummy", 1,  "01");
      bool s3 = r3.CommitOrAbort("abort", "test", "123", "dummy", 1,  "01");

      if(s1 && s2 && s3) {
        cout<<"client 1 success"<<endl;
      } else {
        cout<<"client 1 failure"<<endl;
      }
    }
    */



    //second client
    // get a list of all replica server addresses
    /*
    Json::Value addressChunkrc2_1 = d.FileLookUp(1, "123", "dummy", "01");

    // get votes from all replicas
    ClientReplica rc2_1{addressChunkrc2_1.get("host_urls","")[0].asString()};
    rc2_1.ShowFileContents();

    ClientReplica rc2_2{addressChunkrc2_1.get("host_urls","")[1].asString()};
    rc2_2.ShowFileContents();

    ClientReplica rc2_3{addressChunkrc2_1.get("host_urls","")[2].asString()};
    rc2_3.ShowFileContents();
    */
    /*

    vote1 = rc2_1.GetVote("test2", "123", "dummy", 2,  "01");  
    vote2 = rc2_2.GetVote("test2", "123", "dummy", 2,  "01");
    vote3 = rc2_3.GetVote("test2", "123", "dummy", 2,  "01");

    // send commit/abort to all replicas
    if(vote1 == "commit" && vote1 == vote2 && vote1 == vote3) {
      bool s1 = rc2_1.CommitOrAbort("commit", "test2", "123", "dummy", 2,  "01");
      bool s2 = rc2_2.CommitOrAbort("commit", "test2", "123", "dummy", 2,  "01");
      bool s3 = rc2_3.CommitOrAbort("commit", "test2", "123", "dummy", 2,  "01");

      rc2_1.ShowFileContents();
      rc2_2.ShowFileContents();
      rc2_3.ShowFileContents();

      if(s1 && s2 && s3) {
        cout<<"client 2 success"<<endl;
      } else {
        cout<<"client 2 failure"<<endl;
      }

    } else {
      bool s1 = rc2_1.CommitOrAbort("abort", "test2", "123", "dummy", 2,  "01");
      bool s2 = rc2_2.CommitOrAbort("abort", "test2", "123", "dummy", 2,  "01");
      bool s3 = rc2_3.CommitOrAbort("abort", "test2", "123", "dummy", 2,  "01");

      if(s1 && s2 && s3) {
        cout<<"client 2 success"<<endl;
      } else {
        cout<<"client 2 failure"<<endl;
      }
    }
    */
}

int main() {
  try {
    bothclientconcFirstChunk();
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
}