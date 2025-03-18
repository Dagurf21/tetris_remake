# Compiler and flags
CXX         = g++
CXXFLAGS    = -Wall -std=c++17 -I./include
LDFLAGS     = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Directories
SRCDIR      = src
OBJDIR      = obj
BINDIR      = bin

# Files
SOURCES     = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS     = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SOURCES))
TARGET      = $(BINDIR)/TetrisSFML

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS)
	@mkdir -p $(BINDIR)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean build artifacts
clean:
	rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean
