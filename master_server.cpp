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

  virtual Json::Value FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
  virtual Json::Value CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
};

MasterServer::MasterServer(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

Json::Value MasterServer::FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) { 
  Json::Value result;
  
  if (chunkId == 1) {
    result["host_urls"].append("http://localhost:8384");
    result["host_urls"].append("http://localhost:8385");
    result["host_urls"].append("http://localhost:8386");
  } else{
    result["host_urls"].append("http://localhost:8387");
    result["host_urls"].append("http://localhost:8388");
    result["host_urls"].append("http://localhost:8389");
  }

  return result;
}

Json::Value MasterServer::GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID){
  Json::Value result;
  return result;
}

Json::Value MasterServer::CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID){
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