CXX = g++
CXXFLAGS = -Iinclude -Wall -Wextra -O2

SRC_DIR = src
INC_DIR = include
LIB_DIR = ../lib
BIN_DIR = bin

BINS = drop grab

DROP_SRCS = $(SRC_DIR)/drop.cpp $(SRC_DIR)/server.cpp $(LIB_DIR)/common.cpp
GRAB_SRCS = $(SRC_DIR)/grab.cpp $(SRC_DIR)/client.cpp $(LIB_DIR)/common.cpp

DROP_OBJS = $(BIN_DIR)/drop.o $(BIN_DIR)/server.o $(LIB_DIR)/common.o
GRAB_OBJS = $(BIN_DIR)/grab.o $(BIN_DIR)/client.o $(LIB_DIR)/common.o

all: $(BINS)

drop: $(DROP_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L$(LIB_DIR)

grab: $(GRAB_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -L$(LIB_DIR)

# compile source files into object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(BIN_DIR)/*.o $(BINS)

.PHONY: all clean list
