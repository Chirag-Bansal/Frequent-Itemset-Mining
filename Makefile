CC = g++
CXXFLAGS = -std=c++14

BUILD_TYPE = release

ifeq ($(BUILD_TYPE), debug)
    CXXFLAGS += -g -O3 -Wall
else
    CXXFLAGS += -O3 -DNDEBUG
endif

all: fptree apriori

%: %.cpp
	$(CC) $(CXXFLAGS) $< -o $@ 

clean: 
	rm fptree apriori