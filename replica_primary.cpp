/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    stubserver.cpp
 * @date    02.05.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/
#include <iostream>
#include "gen/abstractstubserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

using namespace jsonrpc;
using namespace std;

class ReplicaPrimary : public AbstractStubServer {
public:
  ReplicaPrimary(AbstractServerConnector &connector, serverVersion_t type);

  virtual Json::Value FileLookUp(const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value  CommitOrAbort(const std::string& action, int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
};

ReplicaPrimary::ReplicaPrimary(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

Json::Value ReplicaPrimary::FileLookUp(const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  return result; 
}

Json::Value ReplicaPrimary::GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  result["status"] = "commit";
  return result; 
}

Json::Value ReplicaPrimary:: CommitOrAbort(const std::string& action, int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID){
  Json::Value result;
  return result; 
}

int main() {
  HttpServer httpserver(8384);
  ReplicaPrimary s(httpserver, JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}