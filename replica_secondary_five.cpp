#include <iostream>
#include "gen/abstractstubserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>
#include <fstream>

using namespace jsonrpc;
using namespace std;

class ReplicaSecondaryFive : public AbstractStubServer {
public:
  ReplicaSecondaryFive(AbstractServerConnector &connector, serverVersion_t type);

  virtual Json::Value FileLookUp(const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value  CommitOrAbort(const std::string& action, int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
};

ReplicaSecondaryFive::ReplicaSecondaryFive(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

Json::Value ReplicaSecondaryFive::FileLookUp(const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  return result; 
}

Json::Value ReplicaSecondaryFive::GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  result["status"] = "commit";
  return result; 
}

Json::Value ReplicaSecondaryFive:: CommitOrAbort(const std::string& action, int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID){
  Json::Value result;
  result["status"] = true;
  return result; 
}

int main() {
  ifstream in("data.txt");
  string chunk2[5] = {};
  int x = 0;
  string line;
  while(getline(in,line) && (x < 10)){
    if (x > 4){
      chunk2[x] = line;
    }
    x++;
  }
  in.close();
  
  HttpServer httpserver(8389);
  ReplicaSecondaryFive s(httpserver, JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}