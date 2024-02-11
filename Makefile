CXX = g++
CXXFLAGS = -std=c++20
LIBS = -lcurl -lgumbo

SRC_FILES = inverted_index.cpp main.cpp search_engine.cpp web_crawler.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)
TARGET = search_engine

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)
