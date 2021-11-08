all:
	g++ -g --std=c++2a -Wall -Wpedantic -O0 -DBOOST_LOG_DYN_LINK qkd.cpp -I -lpthread -pthread -lboost_log -o qkd
