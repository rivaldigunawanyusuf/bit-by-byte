# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Include and library directories
INCLUDE_DIR = src/SDL2/include
LIB_DIR = src/SDL2/lib

# Libraries to link (without SDL2_ttf)
LIBS = -lmingw32 -lSDL2main -lSDL2

# Source and target
SRC = main.cpp
TARGET = number_guessing_game.exe

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -L$(LIB_DIR) -o $(TARGET) $(SRC) $(LIBS)

# Clean target
clean:
	-del $(TARGET) 2>nul
	-del *.o 2>nul

# Run target
run: $(TARGET)
	.\$(TARGET)

.PHONY: all clean run
