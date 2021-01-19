all: client

.PHONY: client

CC = g++
CFLAGS = -g

TCP_CLIENT_OBJECTS = tcp_client.o tcp_client_FSM.o FSM.o state.o myUIUX.o ssl_for_client.o
TCP_CLIENT = tcp_client.cpp appSpecConst.h tcp_client_FSM.h ssl_for_client.h #myUIUX.h
TCP_CLIENT_FSM = tcp_client_FSM.cpp tcp_client_FSM.h FSM.h appSpecConst.h myUIUX.h ssl_for_client.h #state.h 
FSM = FSM.cpp FSM.h state.h
STATE = state.cpp state.h myUIUX.h
MYUIUX = myUIUX.cpp myUIUX.h
SSL_FOR_CLIENT = ssl_for_client.cpp ssl_for_client.h

client: $(TCP_CLIENT_OBJECTS)
	g++ $(TCP_CLIENT_OBJECTS) -o client -lssl -lcrypto
tcp_client.o: $(TCP_CLIENT)
	g++ -c tcp_client.cpp
tcp_client_FSM.o: $(TCP_CLIENT_FSM)
	g++ -c tcp_client_FSM.cpp
FSM.o: $(FSM)
	g++ -c FSM.cpp
state.o: $(STATE)
	g++ -c state.cpp
myUIUX.o: $(MYUIUX)
	g++ -c myUIUX.cpp
ssl_for_client.o: $(SSL_FOR_CLIENT)
	g++ -c ssl_for_client.cpp

clean:
	rm -f myUIUX.o state.o FSM.o tcp_client_FSM.o tcp_client.o ssl_for_client.o
