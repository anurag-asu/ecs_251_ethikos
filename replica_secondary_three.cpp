#include <iostream>
#include "gen/abstractstubserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

using namespace jsonrpc;
using namespace std;

class ReplicaSecondaryThree : public AbstractStubServer {
public:
  ReplicaSecondaryThree(AbstractServerConnector &connector, serverVersion_t type);

  virtual Json::Value FileLookUp(const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value  CommitOrAbort(const std::string& action, int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
};

ReplicaSecondaryThree::ReplicaSecondaryThree(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

Json::Value ReplicaSecondaryThree::FileLookUp(const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  return result; 
}

Json::Value ReplicaSecondaryThree::GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  result["status"] = "commit";
  return result; 
}

Json::Value ReplicaSecondaryThree:: CommitOrAbort(const std::string& action, int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID){
  Json::Value result;
  result["status"] = true;
  return result; 
}

int main() {
  HttpServer httpserver(8387);
  ReplicaSecondaryThree s(httpserver, JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}