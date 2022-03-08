#include <iostream>
#include <thread>
#include <functional>
#include "gen/stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include "client_replica.h"

using namespace jsonrpc;
using namespace std;

HttpClient httpclient("http://localhost:8383");
  // StubClient c(httpclient, JSONRPC_CLIENT_V1); //json-rpc 1.0
StubClient c(httpclient, JSONRPC_CLIENT_V2); // json-rpc 2.0

HttpClient httpclient2("http://localhost:8283");
StubClient d(httpclient, JSONRPC_CLIENT_V2);
void thread() {

}

void execute(ClientReplica r1,ClientReplica r2,ClientReplica r3,const std::string& data, int offset){
  string vote1 = r1.GetVote(data, "123", "dummy", offset,  "01");  
  string vote2 = r2.GetVote(data, "123", "dummy", offset,  "01");
  string vote3 = r3.GetVote(data, "123", "dummy", offset,  "01");
  if(vote1 == "commit" && vote1 == vote2 && vote1 == vote3) {
      bool s1 = r1.CommitOrAbort("commit", data, "123", "dummy", offset,  "01");
      bool s2 = r2.CommitOrAbort("commit", data, "123", "dummy", offset,  "01");
      bool s3 = r3.CommitOrAbort("commit", data, "123", "dummy", offset,  "01");
  }
}

void printresult(ClientReplica r1,ClientReplica r2,ClientReplica r3,int clientNum) {
  cout<<"Displaying data in replicas for client "<<clientNum<<endl;
  r1.ShowFileContents();
  r2.ShowFileContents();
  r3.ShowFileContents();
}
void bothclientconcFirstChunk() {
    //first client
   // get a list of all replica server addresses
    Json::Value addressChunk1 = c.FileLookUp(1, "123", "dummy", "01");

    // get votes from all replicas
    ClientReplica r1{addressChunk1.get("host_urls","")[0].asString()};
    ClientReplica r2{addressChunk1.get("host_urls","")[1].asString()};
    ClientReplica r3{addressChunk1.get("host_urls","")[2].asString()};
    
    printresult(r1,r2,r3,1);

    Json::Value addressChunkrc2_1 = d.FileLookUp(1, "123", "dummy", "01");

    // get votes from all replicas
    ClientReplica rc2_1{addressChunkrc2_1.get("host_urls","")[0].asString()};
    ClientReplica rc2_2{addressChunkrc2_1.get("host_urls","")[1].asString()};
    ClientReplica rc2_3{addressChunkrc2_1.get("host_urls","")[2].asString()};
    
    printresult(rc2_1,rc2_2,rc2_3,2);

    vector<std::thread> threads;

    static std::string file_data[4] = {"client 1 u1", "client 2 u1", "client2 u2", "client1 u2"};
    static int offsets[4] = {1,2,1,0};

    threads.push_back(std::thread(execute,r1,r2,r3,file_data[0],offsets[0]));
    threads.push_back(std::thread(execute,rc2_1,rc2_2,rc2_3,file_data[1],offsets[1]));

    threads.push_back(std::thread(execute,rc2_1,rc2_2,rc2_3,file_data[2],offsets[2]));
    threads.push_back(std::thread(execute,r1,r2,r3,file_data[3],offsets[3]));

    for (auto& t: threads)
      t.join();
  
    printresult(r1,r2,r3,1);
    printresult(rc2_1,rc2_2,rc2_3,2);
}

int main() {
  try {
    bothclientconcFirstChunk();
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
}