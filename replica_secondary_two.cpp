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

File *fileFirstChunkPrivate, *fileFirstChunkPublic;

class ReplicaSecondaryTwo : public AbstractStubServer {
public:
  ReplicaSecondaryTwo(AbstractServerConnector &connector, serverVersion_t type);

  virtual void ShowFileContents();
  virtual Json::Value FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
  virtual Json::Value CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
};

ReplicaSecondaryTwo::ReplicaSecondaryTwo(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

void ReplicaSecondaryTwo::ShowFileContents() {
  cout<<"displaying first chunk's content"<<endl;

  for(int i=0;i<fileFirstChunkPublic->file_rep.size(); i++) {
    cout<<"content at index = "<<i<<": "<<fileFirstChunkPublic->file_rep[i]<<endl;
  }
}

Json::Value ReplicaSecondaryTwo::FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  return result; 
}

Json::Value ReplicaSecondaryTwo::GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID) {
  Json::Value result;
  
  try {
    fileFirstChunkPublic->file_rep[offset] = content;
    result["status"] = "commit";
  } catch(JsonRpcException &e) {
    cout<<e.what();
    result["status"] = "abort";
  }
  return result; 
}

Json::Value ReplicaSecondaryTwo:: CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID){
  Json::Value result;
  result["status"] = true;
  return result; 
}

int main() {

  File fPvt;
  fileFirstChunkPrivate = &fPvt;

  fileFirstChunkPrivate->file_rep.push_back("1");
  fileFirstChunkPrivate->file_rep.push_back("2");
  fileFirstChunkPrivate->file_rep.push_back("3");

  File fPub;
  fileFirstChunkPublic = &fPub;

  fileFirstChunkPublic->file_rep.push_back("1");
  fileFirstChunkPublic->file_rep.push_back("2");
  fileFirstChunkPublic->file_rep.push_back("3");


  HttpServer httpserver(8386);
  ReplicaSecondaryTwo s(httpserver, JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}