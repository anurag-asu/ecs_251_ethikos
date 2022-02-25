#include <iostream>
#include "gen/abstractstubserver.h"
#include "jsonrpccpp/server/connectors/httpserver.h"
#include <stdio.h>

#include "json/json.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/value.h"

using namespace jsonrpc;
using namespace std;

class MasterServer : public AbstractStubServer {
public:
  MasterServer(AbstractServerConnector &connector, serverVersion_t type);

  virtual Json::Value FileLookUp(const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value CommitOrAbort(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
};

MasterServer::MasterServer(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

Json::Value MasterServer::FileLookUp(const std::string& fhandle, const std::string& filename, 
  const std::string& owner_vsID) { 
  Json::Value result;
  
  result["host_urls"].append("http://locahost:8384");
  result["host_urls"].append("http://locahost:8385");
  result["host_urls"].append("http://locahost:8386");

  return result;
}

Json::Value MasterServer::GetVote(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID){
  Json::Value result;
  return result;
}

Json::Value MasterServer::CommitOrAbort(int content, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID){
  Json::Value result;
  return result;
}

int main() {
  HttpServer httpserver(8383);
  MasterServer s(httpserver, JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}