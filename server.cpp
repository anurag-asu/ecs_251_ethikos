#include <bits/stdc++.h>

#include "server.h"
#include "Customer.h"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <stdio.h>

using namespace jsonrpc;
using namespace std;

vector<Customer> customers;

class StoreServer : public Server {
public:
    StoreServer(AbstractServerConnector &connector, serverVersion_t type);

    virtual Json::Value createCustomer(const Json::Value& customerDetails);
    virtual Json::Value assginSecretKey(int custId, const std::string& randomSeed, int storeId);
    virtual Json::Value createOrder(int custId, const Json::Value& orderDetails, int storeId);
    virtual Json::Value decryptOrder(bool custConsent, int custId, int orderId, const std::string& secretKey, int storeId);
    virtual Json::Value returnItem(bool custConsent, int custId, int itemId, int orderId, const std::string& secretKey, int storeId);
    virtual Json::Value generateKey(int custId, int storeId, int seed);
    virtual Json::Value getCategoriesForOrder(int custId, int orderId);
    virtual Json::Value checkForDiscount(int categoryKey, bool custConsent, int custId, int orderId, const std::string& secretKey, int storeId);
};

StoreServer::StoreServer(AbstractServerConnector &connector, serverVersion_t type) : Server(connector, type) {}

Json::Value StoreServer::createCustomer(const Json::Value& custDetails) {
    Json::Value res;
    Customer c(custDetails["firstName"].asString(), custDetails["lastName"].asString());

    customers.push_back(c);
    res["custId"] = c.get_cust_id();
    res["storeId"] = 1;
    
    return res;
};

Json::Value StoreServer::assginSecretKey(int custId, const std::string& randomSeed, int storeId) {
    Json::Value res;
    return res;
};

Json::Value StoreServer::createOrder(int custId, const Json::Value& orderDetails, int storeId) {
    Json::Value res;
    vector<Item> items;
    Json::Value order;

    for(int i=0;i<orderDetails.get("orders", "").size(); i++) {
        order = orderDetails.get("orders", "")[i];
        Item item(order["category"].asString(), order["itemId"].asString(), 
                        order["name"].asString(), order["price"].asString());
        items.push_back(item);
    }

    Order cust_order(items, orderDetails["orderId"].asInt(), 1, orderDetails["key"].asInt());

    for(int i=0;i<customers.size();i++) {
        if(customers[i].get_cust_id() == custId) {
            customers[i].add_order(cust_order);
            break;
        }
    }

    cout<<endl<<endl;
    cout<<"displaying order items encrypted";
    cout<<endl<<endl;

    for(int i=0;i<customers.size();i++) {
        if(customers[i].get_cust_id() == custId) {
            for(Order order: customers[i].get_cust_order()) {
                cout<<order.get_order_info()<<endl;
            }
        }
    }

    cout<<endl<<endl;

    res["orderId"] = orderDetails["orderId"].asInt();
    return res;
};

Json::Value StoreServer::decryptOrder(bool custConsent, int custId, int orderId, const std::string& secretKey, int storeId) {
    Json::Value res;
    return res;
};

Json::Value StoreServer::returnItem(bool custConsent, int custId, int itemId, int orderId, const std::string& secretKey, int storeId) {
    Json::Value res;
    
    cout<<"processing return"<<endl<<endl;


    for(int i=0;i<customers.size();i++) {
        if(customers[i].get_cust_id() == custId) {
            customers[i].process_return(orderId, itemId, stoi(secretKey));
            break;
        }
    }

    cout<<endl<<endl;

    res["status"] = true;
    return res;
};

Json::Value StoreServer::generateKey(int custId, int storeId, int seed){
    Json::Value res;
    cout<<"generating key"<<endl;

    for(int i=0;i<customers.size();i++) {
        if(customers[i].get_cust_id() == custId) {
            res["key"] = customers[i].generate_rand_key(seed);
            break;
        }
    }

    return res;
}

Json::Value StoreServer::getCategoriesForOrder(int custId, int orderId) {
    Json::Value res;
    return res;
}

Json::Value StoreServer::checkForDiscount(int categoryKey, bool custConsent, int custId, int orderId, const std::string& secretKey, int storeId) {
    Json::Value res;
    return res;
};

int main() {
  HttpServer httpserver(8383);
  StoreServer s(httpserver,
                 JSONRPC_SERVER_V1V2); // hybrid server (json-rpc 1.0 & 2.0)
  s.StartListening();
  cout << "Hit enter to stop the server" << endl;
  getchar();

  s.StopListening();

  return 0;
}