CXX := g++
CXXFLAGS := -Wall
SRCDIR := src
BUILDDIR := build

# List of source files
SRCS := $(wildcard $(SRCDIR)/*.cpp)

# Generate object file names from source file names
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Default target for Linux
$(BUILDDIR)/meteor: $(OBJS)
	$(CXX) $(CXXFLAGS) -lm -lraylib $^ -o $@

# Rule to compile each source file into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target for Windows
ifeq ($(filter windows,$(MAKECMDGOALS)),windows)
    CXX := x86_64-w64-mingw32-g++
    # Add MinGW specific flags if necessary
    # CXXFLAGS += <MinGW specific flags>
    # Change linker flags if necessary
    # LDLIBS += <MinGW specific libraries>
    OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%-win.o,$(SRCS))
		MINGW_INCLUDE_DIR := /usr/x86_64-w64-mingw32/include
endif

# Target for Windows
windows: $(BUILDDIR)/meteor.exe
$(BUILDDIR)/%-win.o: $(SRCDIR)/%.cpp
		$(CXX) $(CXXFLAGS) -I$(MINGW_INCLUDE_DIR) -static -lm -lraylib -c $< -o $@

# Rule to link object files into executable
$(BUILDDIR)/meteor.exe: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -static -L./win-lib/ -lraylib -lraylibdll -lopengl32 -lgdi32 -lwinmm -lkernel32 -luser32

# Clean target to remove all files in build directory
clean:
	rm -f $(BUILDDIR)/*
