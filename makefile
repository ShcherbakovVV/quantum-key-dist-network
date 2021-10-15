all:
	g++ --std=c++2a -Wall -Wpedantic -O0 -DBOOST_LOG_DYN_LINK main.cpp src/qkdnetwork.cpp src/vertex.cpp src/edge.cpp src/qkdnode.cpp src/qkdlink.cpp src/qkdpathfinder.cpp -lpthread -lboost_log -o main
