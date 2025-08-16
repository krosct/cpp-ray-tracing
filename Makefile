CXX=g++
CXXFLAGS=-Wall -O3 -g -fdiagnostics-color=always -Wextra -MMD
# CXXFLAGS=-O0 -g -fdiagnostics-color=always -MMD
PARALLELFLAG=-fopenmp
SRC_DIR=src
OBJ_DIR=obj
BUILD_DIR=build
INCLUDE_DIR=include
TEST_DIR=test
DEP_DIR=dep
SRCS=$(wildcard $(SRC_DIR)/*.cpp)
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))
BUILDS=$(wildcard $(BUILD_DIR)/*)
TESTS=$(wildcard $(TEST_DIR)/*)
TESTS_TARGET=$(patsubst $(TEST_DIR)/%,$(BUILD_DIR)/%,$(TESTS))
DEPS=$(patsubst $(OBJ_DIR)/%.o,$(DEP_DIR)/%.d,$(OBJS))

all: $(BUILD_DIR)/main

run: $(BUILD_DIR)/main
	./$<

reset: clean all

$(BUILD_DIR)/main: $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(PARALLELFLAG) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D) $(DEP_DIR)
	$(CXX) $(CXXFLAGS) $(PARALLELFLAG) -I$(INCLUDE_DIR) -c $< -o $@ -MF $(patsubst $(OBJ_DIR)/%.o, $(DEP_DIR)/%.d, $@)

$(TEST_DIR)/%: $(TEST_DIR)/%.cpp
	@mkdir -p $(TEST_DIR)
	@mkdir -p $(TEST_DIR)/$(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@ -lcriterion

clean:
	rm -f -r $(OBJ_DIR) main $(BUILD_DIR) $(TEST_DIR)/$(BUILD_DIR) $(DEP_DIR)

-include $(DEPS)