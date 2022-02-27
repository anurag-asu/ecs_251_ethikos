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
#include "file.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

using namespace jsonrpc;
using namespace std;

File *fileFirstChunkPrivatePrimary, *fileFirstChunkPublicPrimary;

class ReplicaPrimary : public AbstractStubServer {
public:
  ReplicaPrimary(AbstractServerConnector &connector, serverVersion_t type);

  virtual void ShowFileContents();
  virtual Json::Value FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
  virtual Json::Value CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
};

ReplicaPrimary::ReplicaPrimary(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

void ReplicaPrimary::ShowFileContents() {
  cout<<"displaying first chunk's content"<<endl;

  for(int i=0;i<fileFirstChunkPublicPrimary->file_rep.size(); i++) {
    cout<<"content at index = "<<i<<": "<<fileFirstChunkPublicPrimary->file_rep[i]<<endl;
  }
}

Json::Value ReplicaPrimary::FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  return result; 
}

Json::Value ReplicaPrimary::GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID) {
  Json::Value result;
  
  try {
    fileFirstChunkPrivatePrimary->file_rep[offset] = content;
    result["status"] = "commit";
  } catch(JsonRpcException &e) {
    cout<<e.what();
    result["status"] = "abort";
  }
  return result; 
}

Json::Value ReplicaPrimary:: CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID){
  Json::Value result;
  
  try {
    fileFirstChunkPublicPrimary->file_rep[offset] = content;
    result["status"] = true;
  } catch(JsonRpcException &e) {
    cout<<e.what();
    result["status"] = false;
  }
  return result; 
}

int main() {

  File fPvt;
  fileFirstChunkPrivatePrimary = &fPvt;

  fileFirstChunkPrivatePrimary->file_rep.push_back("1");
  fileFirstChunkPrivatePrimary->file_rep.push_back("2");
  fileFirstChunkPrivatePrimary->file_rep.push_back("3");

  File fPub;
  fileFirstChunkPublicPrimary = &fPub;

  fileFirstChunkPublicPrimary->file_rep.push_back("1");
  fileFirstChunkPublicPrimary->file_rep.push_back("2");
  fileFirstChunkPublicPrimary->file_rep.push_back("3");

  HttpServer httpserver(8384);
  ReplicaPrimary s(httpserver, JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}