CXX = g++
CXXFLAGS = -std=c++17 `pkg-config --cflags libmongocxx`
LDFLAGS = `pkg-config --libs libmongocxx`

TARGET = search_engine
SRCS = indexing.cpp main.cpp  # Add your source files here
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Rule to compile .cpp files into .o object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean target to remove object files and the executable
clean:
	rm -f $(OBJS) $(TARGET)

# Phony target to prevent conflicts with files named "clean"
.PHONY: clean
