all: server

server:test.o socket_server.o controller.o hardwareModule.o
	g++  test.o socket_server.o controller.o hardwareModule.o -o server -pthread

test.o: test.cpp
	g++ -c test.cpp -pthread

socket_server.o: socket_server.cpp
	g++  -c socket_server.cpp

controller.o: controller/controller.cpp
	g++ -c controller/controller.cpp 
hardwareModule.o: controller/hardwareModule.cpp
	g++ -c controller/hardwareModule.cpp


clean:
	rm -rf *o hello
