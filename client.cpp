/*************************************************************************
 * libjson-rpc-cpp
 *************************************************************************
 * @file    stubclient.cpp
 * @date    01.05.2013
 * @author  Peter Spiess-Knafl <dev@spiessknafl.at>
 * @license See attached LICENSE.txt
 ************************************************************************/

#include <iostream>

#include "storeclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
#include "utils.h"

using namespace jsonrpc;
using namespace std;

int main() {
  HttpClient httpclient("http://localhost:8383");
  // StubClient c(httpclient, JSONRPC_CLIENT_V1); //json-rpc 1.0
  StoreClient c(httpclient, JSONRPC_CLIENT_V2); // json-rpc 2.0

  // create a customer with no key
  Json::Value c1 = c.createCustomer(getCustomerDetails("a", "b", false));
  cout<<"customer created successfully"<<endl;
  cout<<c1.toStyledString()<<endl;

  // customer has no key....so assign a key first, key is returned in response
  cout<<"Please input seed for key generation"<<endl;
  int seed;
  cin>>seed>>endl;
  Json::Value k1 = c.generateKey(c1["custId"].asInt(), c1["storeId"].asInt(), seed);


  // create a order for a customer, using the key assign above
  // replace 123 with the key returned above
  Json::Value o1 = c.createOrder(c1["custId"].asInt(), getOrderDetails(1, k1["key"].asInt()), c1["storeId"].asInt());
  cout<<"order created successfully"<<endl;
  cout<<o1.toStyledString()<<endl;

  // returning an item for a customer
  // replace 123 with the assigned secret key above
  Json::Value r1 = c.returnItem(true, c1["custId"].asInt(), 102, o1["orderId"].asInt(), k1["key"].asInt(), 1);
  cout<<"return processed successfully"<<endl;
  cout<<r1.toStyledString();

  // make a new customer with key already present, make order of your choice and look for discount 


  try {
  } catch (JsonRpcException &e) {
    cerr << e.what() << endl;
  }
}

