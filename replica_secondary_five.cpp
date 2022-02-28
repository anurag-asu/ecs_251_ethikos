#include <iostream>
#include "gen/abstractstubserver.h"
#include "file.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

using namespace jsonrpc;
using namespace std;

File *fileSecondChunkPrivate, *fileSecondChunkPublic;

class ReplicaSecondaryFive : public AbstractStubServer {
public:
  ReplicaSecondaryFive(AbstractServerConnector &connector, serverVersion_t type);

  virtual void ShowFileContents();
  virtual Json::Value FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID);
  virtual Json::Value GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
  virtual Json::Value CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID);
};

ReplicaSecondaryFive::ReplicaSecondaryFive(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

void ReplicaSecondaryFive::ShowFileContents() {
  cout<<"displaying second chunk's content"<<endl;

  for(int i=0;i<fileSecondChunkPublic->file_rep.size(); i++) {
    cout<<"content at index = "<<i<<": "<<fileSecondChunkPublic->file_rep[i]<<endl;
  }
}

Json::Value ReplicaSecondaryFive::FileLookUp(int chunkId, const std::string& fhandle, const std::string& filename, const std::string& owner_vsID) {
  Json::Value result;
  return result; 
}

Json::Value ReplicaSecondaryFive::GetVote(const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID) {
  Json::Value result;
  
  try {
    fileSecondChunkPrivate->file_rep[offset] = content;
    result["status"] = "commit";
  } catch(JsonRpcException &e) {
    cout<<e.what();
    result["status"] = "abort";
  }
  return result; 
}

Json::Value ReplicaSecondaryFive:: CommitOrAbort(const std::string& action, const std::string& content, const std::string& fhandle, const std::string& filename, int offset, const std::string& owner_vsID){
  Json::Value result;
  
  try {
    fileSecondChunkPrivate->file_rep[offset] = content;
    result["status"] = true;
  } catch(JsonRpcException &e) {
    cout<<e.what();
    result["status"] = false;
  }
  return result; 
}

int main() {

  File fPvt5;
  fileSecondChunkPrivate = &fPvt5;

  fileSecondChunkPrivate->file_rep.push_back("1");
  fileSecondChunkPrivate->file_rep.push_back("2");
  fileSecondChunkPrivate->file_rep.push_back("3");

  File fPub5;
  fileSecondChunkPublic = &fPub5;

  fileSecondChunkPublic->file_rep.push_back("1");
  fileSecondChunkPublic->file_rep.push_back("2");
  fileSecondChunkPublic->file_rep.push_back("3");
  
  HttpServer httpserver(8389);
  ReplicaSecondaryFive s(httpserver, JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}