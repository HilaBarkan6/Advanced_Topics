CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror -pedantic -rdynamic

SRC = $(wildcard ../UserCommon/*.cpp) $(wildcard ../UserCommon/configuration/*.cpp) $(wildcard src/*.cpp) 
OBJ = $(SRC:.cpp=.o)

TARGET = simulator_209399021_208239152

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
	$(CXX) $(CXXFLAGS) $(OBJ) -o $@ -ldl -pthread

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ) $(TARGET) > $(NULL) 2>&1 || exit 0

.PHONY: all clean