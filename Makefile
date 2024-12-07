.PHONY: all

CXX = g++
CC = gcc
CXXFLAGS = -Wall -std=c++17
CFLAGS = -Wall -DSQLITE_THREADSAFE=1 -DSQLITE_OMIT_LOAD_EXTENSION

TARGET = authenticate

SRCS = authenticate.cpp sqlite3.c
OBJS = authenticate.o sqlite3.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

sqlite3.o: sqlite3.c
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean: Makefile 
	rm -f *.o
	rm -f authenticate
	rm -f *~