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

  virtual void connectToMaster();
  virtual void connectToPrimaryReplica();
};

ReplicaPrimary::ReplicaPrimary(AbstractServerConnector &connector, serverVersion_t type) : AbstractStubServer(connector, type) {}

void ReplicaPrimary::connectToPrimaryReplica() { cout << "client got connected to primary replica" << endl; }

int main() {
  HttpServer httpserver(8384);
  ReplicaPrimary s(httpserver,
                 JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  s.StartListening();
  cout << "hit ctrl+c to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}