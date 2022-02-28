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

File *fileFirstChunkPrivateSecondaryOne, *fileFirstChunkPublicSecondaryOne;

class ReplicaSecondaryOne : public AbstractStubServer {
public:
  ReplicaSecondaryOne(AbstractServerConnector &connector, serverVersion_t type);

  virtual void ShowFileContents();
  virtual Json::Value FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
  virtual Json::Value CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
};

ReplicaSecondaryOne::ReplicaSecondaryOne(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

void ReplicaSecondaryOne::ShowFileContents() {
  cout<<"displaying first chunk's content"<<endl;

  for(int i=0;i<fileFirstChunkPublicSecondaryOne->file_rep.size(); i++) {
    cout<<"content at index = "<<i<<": "<<fileFirstChunkPublicSecondaryOne->file_rep[i]<<endl;
  }
}

Json::Value ReplicaSecondaryOne::FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  return result; 
}

Json::Value ReplicaSecondaryOne::GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID) {
  Json::Value result;
  
  try {
    fileFirstChunkPrivateSecondaryOne->file_rep[offset] = content;
    result["status"] = "commit";
  } catch(JsonRpcException &e) {
    cout<<e.what();
    result["status"] = "abort";
  }
  return result; 
}

Json::Value ReplicaSecondaryOne:: CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID){
  Json::Value result;
  
  try {
    fileFirstChunkPublicSecondaryOne->file_rep[offset] = content;
    result["status"] = true;
  } catch(JsonRpcException &e) {
    cout<<e.what();
    result["status"] = false;
  }
  return result;  
}

int main() {

  File fPvt21;
  fileFirstChunkPrivateSecondaryOne = &fPvt21;

  fileFirstChunkPrivateSecondaryOne->file_rep.push_back("1");
  fileFirstChunkPrivateSecondaryOne->file_rep.push_back("2");
  fileFirstChunkPrivateSecondaryOne->file_rep.push_back("3");

  File fPub21;
  fileFirstChunkPublicSecondaryOne = &fPub21;

  fileFirstChunkPublicSecondaryOne->file_rep.push_back("1");
  fileFirstChunkPublicSecondaryOne->file_rep.push_back("2");
  fileFirstChunkPublicSecondaryOne->file_rep.push_back("3");


  HttpServer httpserver(8385);
  ReplicaSecondaryOne s(httpserver, JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}