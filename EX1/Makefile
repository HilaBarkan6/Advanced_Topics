CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic


SRC = main.cpp $(wildcard src/*.cpp) $(wildcard src/game_objects/*.cpp)

TARGET = tank_game.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
