# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# SDL2 configuration
SDL2_CFLAGS = -I"d:/bit-by-byte/C++/MandelbrotSet/src/SDL2/include"
SDL2_LIBS = -L"d:/bit-by-byte/C++/MandelbrotSet/src/SDL2/lib" -lmingw32 -lSDL2main -lSDL2

# Target executable
TARGET = mandelbrot_cpp.exe

# Source files
SOURCES = main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(SDL2_LIBS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(SDL2_CFLAGS) -c $< -o $@

# Clean build files
clean:
	del /f $(OBJECTS) $(TARGET)

# Run the program
run: $(TARGET)
	copy "d:/bit-by-byte/C++/MandelbrotSet/SDL2.dll" ".\SDL2.dll"
	.\$(TARGET)

# Help target
help:
	@echo Available targets:
	@echo   all     - Build the executable
	@echo   clean   - Remove build files
	@echo   run     - Build and run the program
	@echo   help    - Show this help message

.PHONY: all clean run help
