TARGET = grab

SRC_DIR = src
INC_DIR = include
BIN_DIR = bin
LIB_DIR = ../lib

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRCS))

LIB_SRCS = $(wildcard $(LIB_DIR)/*.cpp)
LIB_OBJS = $(patsubst $(LIB_DIR)/%.cpp,$(BIN_DIR)/%.o,$(LIB_SRCS))

CXX = g++
CXXFLAGS = -I$(INC_DIR) -I$(LIB_DIR) -Wall -Wextra
LDFLAGS = -L$(LIB_DIR)

all: $(TARGET)

$(TARGET): $(OBJS) $(LIB_OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

.PHONY: all clean
