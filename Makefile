CXX = g++
CXXFLAGS = -std=c++20
LIBS = -lcurl -lgumbo

# Adjust the number based on the available CPU cores
JOBS = 4

SRC_FILES = inverted_index.cpp server.cpp search_engine.cpp web_crawler.cpp main.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)
TARGET = search_engine

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

.PHONY: all clean

clean:
	rm -f $(OBJ_FILES) $(TARGET)

# Add the following rules for optimization and caching

optimize:
	$(CXX) $(CXXFLAGS) -O2 -c $(SRC_FILES)
	$(CXX) $(CXXFLAGS) -O2 $(OBJ_FILES) -o $(TARGET) $(LIBS) $(flto)

# Use CCache for caching compiled objects
ccache:
	CXX="ccache $(CXX)" make -j$(JOBS)

# Combine optimization and caching
optimize_and_cache:
	CXX="ccache $(CXX)" make optimize
