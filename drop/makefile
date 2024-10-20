$(shell mkdir -p bin)
LDFLAGS = -lpthread
CXXFLAGS = -std=c++17 -g
CFLAGS = -O2 -Iinclude
CPPFLAGS = -Iinclude
SRC_DIR := src
OBJ_DIR := bin
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

drop: $(OBJ_FILES) include/*
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf bin
	rm -f drop
