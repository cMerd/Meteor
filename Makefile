CXX := g++
CXXFLAGS := -Wall
SRCDIR := src
BUILDDIR := build
LIBS := -lm -lraylib

# List of source files
SRCS := $(wildcard $(SRCDIR)/*.cpp)

# Generate object file names from source file names
OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SRCS))

# Default target
all: $(BUILDDIR)/meteor

# Rule to link object files into executable
$(BUILDDIR)/meteor: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@

# Rule to compile each source file into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target for cheats
cheat: CXXFLAGS += -D_METEOR_BUILD_WITH_CHEATS_
cheat: $(BUILDDIR)/meteor_cheat

$(BUILDDIR)/meteor_cheat: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LIBS) -o $@

# Windows target setup
ifeq ($(findstring windows,$(MAKECMDGOALS)),windows)
    CXX := x86_64-w64-mingw32-g++
    LIBS := -static -L./win-lib/ -lraylib -lraylibdll -lopengl32 -lgdi32 -lwinmm -lkernel32 -luser32

    OBJS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%-win.o,$(SRCS))
    MINGW_INCLUDE_DIR := /usr/x86_64-w64-mingw32/include

    # Windows-specific rule to compile
    $(BUILDDIR)/%-win.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) -I$(MINGW_INCLUDE_DIR) -static -c $< -o $@

    # Windows-specific linking
    $(BUILDDIR)/meteor.exe: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LIBS)
endif

# Target for Windows builds
windows: $(BUILDDIR)/meteor.exe

# Clean target to remove all files in build directory
clean:
	rm -f $(BUILDDIR)/*
