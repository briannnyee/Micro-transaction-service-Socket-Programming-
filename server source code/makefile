all:server

.PHONY: server

CC = g++
CFLAGS = -lssl -lcrypto
TAR_OBJ = tcp_server.o client_and_user.o thread_pool.o server.o user_map.o
TCP_SERVER = tcp_server.cpp server.h
SERVER_OBJ = server.cpp server.h
THREAD_POOL = thread_pool.cpp thread_pool.h
USER_MAP = user_map.cpp user_map.h
CLIENT_AND_USER = client_and_user.cpp client_and_user.h

server: ${TAR_OBJ}
	${CC} $^ -o $@ -pthread -lssl -lcrypto

tcp_server.o: ${TCP_SERVER}
	${CC} ${CFLAGS} -c $<

server.o: ${SERVER_OBJ}
	${CC} ${CFLAGS} -c $<

thread_pool.o: ${THREAD_POOL}
	${CC} ${CFLAGS} -c $<

user_map.o: ${USER_MAP}
	${CC} ${CFLAGS} -c $<

client_and_user.o: ${CLIENT_AND_USER}
	${CC} ${CFLAGS} -c $<

clean: 
	rm -rf *.o *.h.gch