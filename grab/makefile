$(shell mkdir -p bin)
LDFLAGS = -lpthread
CXXFLAGS = -std=c++17
CFLAGS = -O2 -Iinclude -g
CPPFLAGS = -Iinclude
SRC_DIR := src
OBJ_DIR := bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

grab: $(OBJ_FILES) include/*
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf bin
	rm -f drop
