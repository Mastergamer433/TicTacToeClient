.PHONY: clean run
CXX := g++
CXXFLAGS :=
LINKFLAGS := 
OBJS_DIR? = obj/
OBJS_DIR := $(OBJS_DIR)client/
BUILD_DIR? = build/
SOURCE_DIR := .
EXEC := client
SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.cpp)
SOURCE_HEADERS := $(wildcard $(SOURCE_DIR)/*.h)
OBJS := $(patsubst $(SOURCE_DIR)/%.cpp,$(OBJS_DIR)/client/%.o,$(SOURCE_FILES))

$(EXEC): $(OBJS)
	mkdir $(OBJS_DIR)/client -p
	$(CXX) $(OBJS) $(CXXFLAGS) $(LINKFLAGS) -o $(BUILD_DIR)/$(EXEC)

$(OBJS_DIR)/client/%.o : $(SOURCE_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

#Files to be compiled
$(OBJS_DIR)/main.o: $(SOURCE_FILES) $(SOURCE_HEADERS)

run:
	./build/main
clean:
	rm $(OBJS_DIR)/client/*.o 
	rm $(BUILD_DIR)/$(EXEC)
