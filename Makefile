
# Compiler

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# Directories

BIN_DIR = bin
BUILD_DIR = build
SRC_DIR = src
SCENARIO_DIR = src/scenarios

# Lists

SCENARIOS = $(wildcard src/scenarios/*.cpp)
BINARIES = $(patsubst src/scenarios/%.cpp, bin/%, $(SCENARIOS))

COMMON_SRC = $(wildcard src/*.cpp)
COMMON_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMMON_SRC))

# Build

all: $(BINARIES)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(SCENARIO_DIR)/%.cpp $(COMMON_OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< $(COMMON_OBJ) -o $@

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

# Clean

.PHONY: all clean
.SECONDARY:

clean:
	rm -rf $(BIN_DIR) $(BUILD_DIR)
