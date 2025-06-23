CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -fPIC

SRC = $(wildcard src/*.cpp) $(wildcard ../UserCommon/*.cpp)
OBJ = $(SRC:.cpp=.o)

TARGET = GameManager_<your_ids>.so

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