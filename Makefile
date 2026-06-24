CFLAGS+= -Wall -Werror -std=gnu99 -g 
LDFLAGS= -lm 

SRC_DIR   = src
BUILD_DIR = build
TOOLS_DIR = tools
DOC_DIR   = doc

TEST_BIN = $(BUILD_DIR)/test_runner
TEST_DIR = tests
GENERATED_RUNNER = $(BUILD_DIR)/test_runner.c

HW=aposem
BINARIES = $(BUILD_DIR)/aposem-main

CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs)

# Seznam vsech objektovych souboru vygenerovanych ze zdrojaku .c
SRCS = $(wildcard ${SRC_DIR}/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_CORE_MODULES = $(filter-out src/aposem-main.c, $(wildcard src/*.c))

RED    := $(shell printf '\033[0;31m')
GREEN  := $(shell printf '\033[0;32m')
YELLOW := $(shell printf '\033[0;33m')
BLUE   := $(shell printf '\033[0;34m')
PURPLE := $(shell printf '\033[0;35m')
CYAN   := $(shell printf '\033[0;36m')
BOLD   := $(shell printf '\033[1m')
RESET  := $(shell printf '\033[0m')

.PHONY: all docs zip clean run help test

all: install ${BINARIES}

$(BUILD_DIR) $(DOC_DIR):
	@mkdir -p $@

$(BUILD_DIR)/aposem-main: ${OBJS} | ${BUILD_DIR}
	${CC} ${OBJS} ${LDFLAGS} -o $@

${OBJS}: $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	${CC} -c ${CFLAGS} $< -o $@

test: install | $(BUILD_DIR)
	@printf "$(YELLOW)Generating test runner...$(RESET)\n"
	./tools/generate_tests.sh $(TEST_DIR) $(GENERATED_RUNNER)
	
	@printf "$(CYAN)Compiling tests...$(RESET)\n"
	$(CC) $(CFLAGS) $(GENERATED_RUNNER) $(TEST_SRCS) $(TEST_CORE_MODULES) $(LDFLAGS) -o $(TEST_BIN)
	
	@printf "$(YELLOW)Running tests under Valgrind...$(RESET)\n"
	valgrind --leak-check=full --error-exitcode=1 ./$(TEST_BIN)
	@printf "$(GREEN)Test runner finished.$(RESET)\n"

install: dependencies.txt
	./${TOOLS_DIR}/install_dependencies.sh
	@touch install

docs: ${SRCS} | ${DOC_DIR}
	doxygen Doxyfile
	@echo "Documentation generated in ${DOC_DIR}/html/index.html"

zip:
	zip ${HW}-brute.zip nucleo/prgsem-mbed.cpp assets/*.ppm ${SRC_DIR}/*.c ${SRC_DIR}/*.h Makefile README.md Doxyfile
	@echo "Created ${HW}-brute.zip"

clean:
	rm -f  install
	rm -f  ${HW}-brute.zip
	rm -rf ${DOC_DIR}
	rm -rf ${BUILD_DIR}

run: all
	./${BINARIES}

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  all       - Build the project and install dependencies"
	@echo "  install   - Install dependencies"
	@echo "  test      - Run the test suite under Valgrind"
	@echo "  docs      - Generate documentation using Doxygen"
	@echo "  zip       - Create a zip archive of the project"
	@echo "  clean     - Remove build artifacts and documentation"
	@echo "  run       - Build and run the project"
	@echo "  help      - Show this help message"