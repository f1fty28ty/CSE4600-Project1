# Makefile - Build script for myshell

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = myshell
SOURCES = main.cpp myshell.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp myshell.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all clean
