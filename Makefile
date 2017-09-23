CXX = g++
CXXFLAGS = -Wall -Wextra
CPPFLAGS = -DGEN_LG -DGEN_HVAC_PANASONIC
BUILD_DIR = cmake-build-debug
COMMON_SOURCES = IRGen.cpp BWCLS.cpp
OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(wildcard *.cpp))
COMMON_OBJECTS = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(COMMON_SOURCES))
HEADERS = $(wildcard *.h)

.PHONY: all clean

all: $(shell mkdir -p $(BUILD_DIR)) $(OBJECTS) IR-lg-ac-demo IR-panasonic-ac-demo

$(BUILD_DIR)/%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(EXTRA_FLAGS) -c $< -o $@

IR-lg-ac-demo: $(COMMON_OBJECTS) $(BUILD_DIR)/IR-lg-ac-demo.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMMON_OBJECTS) $(BUILD_DIR)/IR-lg-ac-demo.o -o $(BUILD_DIR)/$@

IR-panasonic-ac-demo: $(COMMON_OBJECTS) $(BUILD_DIR)/IR-panasonic-ac-demo.o
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(COMMON_OBJECTS) $(BUILD_DIR)/IR-panasonic-ac-demo.o -o $(BUILD_DIR)/$@

clean:
	-rm -rf $(BUILD_DIR)