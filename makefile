CXX = g++
CXXFLAGS = --std=c++2a -O0 -Wall -Wpedantic -pthread
CPPFLAGS = -I/usr/include -I/usr/include/SDL2
LDFLAGS = -L/usr/lib/x86_64-linux-gnu
BOOST_FLAGS = -D_REENTRANT -DBOOST_LOG_DYN_LINK -lpthread -lboost_log
SDL_FLAGS = -lSDL2 -lSDL2main -lSDL2_image

all: clean qkd

clean:
	rm -f qkd graph image

qkd:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) qkd.cpp -o qkd $(LDFLAGS) $(BOOST_FLAGS) $(SDL_FLAGS)
