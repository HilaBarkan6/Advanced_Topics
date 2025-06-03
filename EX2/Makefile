CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -g

SRC = main.cpp $(wildcard src/*.cpp) $(wildcard src/game_objects/*.cpp) $(wildcard src/implementations/*.cpp)
SRC_NO_MAIN = $(filter-out main.cpp, $(SRC))
TEST_SRC = test/tests.cpp

TARGET = tank_game.exe
TEST_TARGET = test_runner.exe

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC) $(SRC_NO_MAIN)
	$(CXX) $(CXXFLAGS) $(TEST_SRC) $(SRC_NO_MAIN) -lgtest -lgtest_main -pthread -o $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

.PHONY: all clean test
