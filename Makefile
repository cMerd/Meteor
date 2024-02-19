CXX := g++
CXXFLAGS := -Wall -std=c++20
SRCDIR := src
BUILDDIR := build

# List of source files
SRCS := $(wildcard $(SRCDIR)/*.cpp)

# Generate object file names from source file names
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Target for compiling for Linux
$(BUILDDIR)/meteor: $(OBJS)
	$(CXX) $(CXXFLAGS) -lm -lraylib $^ -o $@

# Rule to compile each source file into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove all files in build directory
clean:
	rm -f $(BUILDDIR)/*


