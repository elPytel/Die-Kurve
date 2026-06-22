CFLAGS+= -Wall -Werror -std=gnu99 -g 
LDFLAGS= -lm 

SRC_DIR = src
BUILD_DIR = build
TOOLS_DIR=tools
DOC_DIR=doc

HW=aposem
BINARIES = $(BUILD_DIR)/aposem-main

CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs)

# Seznam vsech objektovych souboru vygenerovanych ze zdrojaku .c
SRCS=$(wildcard ${SRC_DIR}/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all docs zip clean run help

all: install ${BINARIES}

$(BUILD_DIR) $(DOC_DIR):
	@mkdir -p $@

$(BUILD_DIR)/aposem-main: ${OBJS} | ${BUILD_DIR}
	${CC} ${OBJS} ${LDFLAGS} -o $@

${OBJS}: $(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	${CC} -c ${CFLAGS} $< -o $@

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
	rm -f ${BINARIES} ${OBJS} install
	rm -f ${HW}-brute.zip
	rm -rf ${DOC_DIR}
	rm -rf ${BUILD_DIR}

run: all
	./${BINARIES}

help:
	@echo "Usage: make [target]"
	@echo "Targets:"
	@echo "  all       - Build the project and install dependencies"
	@echo "  install   - Install dependencies"
	@echo "  docs      - Generate documentation using Doxygen"
	@echo "  zip       - Create a zip archive of the project"
	@echo "  clean     - Remove build artifacts and documentation"
	@echo "  run       - Build and run the project"
	@echo "  help      - Show this help message"