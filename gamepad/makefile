CXX = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lSDL2

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=obj/%.o)
TARGET = gamepad

$(shell mkdir -p obj)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

obj/%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

clean:
	rm -rf obj $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
