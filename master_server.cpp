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

class MasterServer : public AbstractStubServer {
public:
  MasterServer(AbstractServerConnector &connector, serverVersion_t type);

  virtual void connectToMaster();
  virtual void connectToPrimaryReplica();
};

MasterServer::MasterServer(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

void MasterServer::connectToMaster() { cout << "client got connected" << endl; }

int main() {
  HttpServer httpserver(8383);
  MasterServer s(httpserver,
                 JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}