CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -g -fPIC

SRC = $(wildcard src/game_objects/*.cpp)  $(wildcard ../UserCommon/configuration/*.cpp) $(wildcard ../UserCommon/Logger/*.cpp) $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

TARGET = GameManager_209399021_208239152.so

INCLUDES = -I../Common -I../UserCommon

ifeq ($(OS),Windows_NT)
    RM = del /Q
    NULL = NUL
else
    RM = rm -f
    NULL = /dev/null
endif

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) -shared $(CXXFLAGS) $(OBJ) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TARGET) > $(NULL) 2>&1 || exit 0

.PHONY: all clean