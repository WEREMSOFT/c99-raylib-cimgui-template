#////////////////////
# check which OS Am I
#////////////////////

ifeq ($(OS),Windows_NT)
	DETTECTED_OS := $(OS)
else
	UNAME_S := $(shell uname -s)
	DETTECTED_OS := $(UNAME_S)
endif

#//////////////////////////////
# Define some usefull variables
#//////////////////////////////

# Vars for binary build
CC := gcc
SRC_D := src/
BLD_D := bin/
OBJ_D := obj/
ASST_D := assets/
TEST_SRC_D := tests/
TEST_BLD_D := $(TEST_SRC_D)bin/
LIBS_D := libs/
HTML_D := docs/
ASM_D := asm/

SRC_FILES := $(wildcard $(SRC_D)*.c)

INCLUDE_D := -I$(LIBS_D)include/
STATIC_LIBS_D := -L$(LIBS_D)static/
CFLAGS := -O0 -Wpedantic -g -Wall -std=c11 -g3 -D_FORTIFY_SOURCE=2 -DOS_$(DETTECTED_OS) 
DEBUGGER := kdbg # Other options: cgdb gdb
MK_DIR:= mkdir -p
BIN_EXTENSION = bin

# Vars for emscripten build
RAYLIB_PATH := /Users/pabloweremczuk/Documents/Proyectos/c/raylib
EMSC_CFLAGS := -O2 -s -Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -s DISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0 -g4 -s USE_GLFW=3 -s TOTAL_MEMORY=67108864 -v -D PLATFORM_WEB
EMSC_CC := emcc
EMSC_STATIC_LIBS_D := $(LIBS_D)static/libraylib.bc $(LIBS_D)static/cimgui.bc 
#$(LIBS_D)static/imgui_draw.bc $(LIBS_D)static/imgui_widgets.bc

# Call to compilers / linkers
CC_COMMAND := $(CC) $(CFLAGS) $(INCLUDE_D) $(STATIC_LIBS_D)
EMSC_CC_COMMAND := $(EMSC_CC) $(EMSC_CFLAGS) $(INCLUDE_D) $(STATIC_LIBS_D)

#//////////////////////////////////////////////////
# Set static libraries depending on de dettected OS
#//////////////////////////////////////////////////

ifeq ($(DETTECTED_OS),Linux)
	LINK_LIBS := -l:libraylib.a -l:libcimgui_static.a -l:libglfw3.a -lstdc++ -lm -ldl -lpthread -lX11 -lxcb -lGL -lGLX -lXext -lGLdispatch -lXau -lXdmcp
	TEST_LINK_LIBS := -lunity 
else ifeq ($(DETTECTED_OS),Darwin)
	LINK_LIBS := 
	#LINK_LIBS := -O0 -framework IOKit -v -lraylib -framework OpenGL -framework cocoa 
endif

#//////////////	
# Build Targets
#//////////////

.PHONY: main all web test run_% debug_optimized debug_unoptimized print_information create_folder_structure run_html_u run_html_o run_performance_test init_project

all: print_information main web

$(OBJ_D)%.o: $(SRC_D)%.c
	$(CC_COMMAND) -o $(OBJ_D)$@ $^ $(LINK_LIBS)

$(TEST_BLD_D)%.spec.$(BIN_EXTENSION): $(TEST_SRC_D)%.spec.c
	@echo "### Building tests for $(@) START ###"
	$(CC_COMMAND) -o $@ $^ $(TEST_LINK_LIBS) $(LINK_LIBS)
	@echo "### End ###"
	@echo ""

$(BLD_D)%.$(BIN_EXTENSION): $(SRC_D)%.c
	@echo "### Building tests for $(@) START ###"
	$(CC_COMMAND) -o $@ $^ $(LINK_LIBS)
	@echo "### End ###"
	@echo ""

web: $(HTML_D)main.html
	mv $(HTML_D)main.html $(HTML_D)index.html
	cp -r src $(HTML_D)

$(HTML_D)%.html: $(SRC_D)%.c
	$(EMSC_CC_COMMAND) -g4 --source-map-base https://weremsoft.github.io/c99-raylib-cimgui-template/ $^ -o $@ $(EMSC_STATIC_LIBS_D)

print_information:
	@echo "Dettected OS: $(DETTECTED_OS)"

create_folders:
	$(MK_DIR) $(BLD_D)
	$(MK_DIR) $(SRC_D)
	$(MK_DIR) libs/include
	$(MK_DIR) libs/static
	$(MK_DIR) $(HTML_D)
	$(MK_DIR) $(TEST_BLD_D)
	$(MK_DIR) $(ASM_D)
	
init_project: create_folders
	touch ./src/main.c

clean:
	rm -rf $(BLD_D)*
	rm -rf $(HTML_D)*
	rm -rf $(OBJ_D)*
	rm -rf $(TEST_BLD_D)*
	rm -rf $(ASM_D)*

main: $(SRC_FILES)
	$(CC_COMMAND) -o $(BLD_D)$@.bin $^ $(LINK_LIBS)

run_perf_%.$(BIN_EXTENSION): $(BLD_D)%.$(BIN_EXTENSION)
	perf stat -e task-clock,cycles,instructions,cache-references,cache-misses $^
	
debug_%: $(BLD_D)%.$(BIN_EXTENSION)
	$(DEBUGGER) $^

run_%: $(BLD_D)%.$(BIN_EXTENSION)
	$^

test_%: $(TEST_BLD_D)%.spec.$(BIN_EXTENSION)
	$^

$(ASM_D)%.S: $(SRC_D)%.c
	$(CC_COMMAND) -o $@ $(CFLAGS) -S $^ $(LINK_LIBS)  