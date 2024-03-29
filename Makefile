CC = g++ -std=c++14

CFLAGS = -g -I/usr/include/jsoncpp
THREADCFLAG = -pthread

LDFLAGS_SV = -ljsoncpp -lmicrohttpd -ljsonrpccpp-common -ljsonrpccpp-server
LDFLAGS_CL = -ljsoncpp -lcurl -ljsonrpccpp-common -ljsonrpccpp-client

all: abstractstubserver.h stubclient.h masterserver replicaprimary replicasecondaryone replicasecondarytwo replicasecondarythree replicasecondaryfour replicasecondaryfive client clienttwo clientserialdemo clientconcurrentdemo

abstractstubserver.h: spec.json
	jsonrpcstub spec.json --cpp-server=AbstractStubServer
	mv abstractstubserver.h gen

stubclient.h: abstractstubserver.h 
	jsonrpcstub spec.json --cpp-client=StubClient
	mv stubclient.h gen

masterserver: stubclient.h
	${CC} ${CFLAGS} master_server.cpp ${LDFLAGS_SV} -o masterserver

replicaprimary: masterserver
	${CC} ${CFLAGS} replica_primary.cpp ${LDFLAGS_SV} -o replicaprimary

replicasecondaryone: replicaprimary
	${CC} ${CFLAGS} replica_secondary_one.cpp ${LDFLAGS_SV} -o replicasecondaryone

replicasecondarytwo: replicasecondaryone
	${CC} ${CFLAGS} replica_secondary_two.cpp ${LDFLAGS_SV} -o replicasecondarytwo

replicasecondarythree: replicasecondarytwo
	${CC} ${CFLAGS} replica_secondary_three.cpp ${LDFLAGS_SV} -o replicasecondarythree

replicasecondaryfour: replicasecondarythree
	${CC} ${CFLAGS} replica_secondary_four.cpp ${LDFLAGS_SV} -o replicasecondaryfour

replicasecondaryfive: replicasecondaryfour
	${CC} ${CFLAGS} replica_secondary_five.cpp ${LDFLAGS_SV} -o replicasecondaryfive

client: replicasecondaryfive
	${CC} ${CFLAGS} client_replica.cpp client.cpp ${LDFLAGS_CL} -o client

clienttwo: client
	${CC} ${CFLAGS} client_replica.cpp client_two.cpp ${LDFLAGS_CL} -o clienttwo

clientserialdemo: clienttwo
	${CC} ${CFLAGS} client_replica.cpp client_serial_demo.cpp ${LDFLAGS_CL} -o clientserialdemo

clientconcurrentdemo: clienttwo
	${CC} ${CFLAGS} ${THREADCFLAG} client_replica.cpp client_concurrent_demo.cpp ${LDFLAGS_CL} -o clientconcurrentdemo
	