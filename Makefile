CC=gcc

CFLAGS = -Wall -Wextra -std=c99 

LIBS = -lm

BUILD_DIR = build
OUTPUT_DIR = .

SRC_FILES = test.c gw_knuth.c sample_wor.c

all: test

test: $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ $(LIBS) -o test

test_debug: $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -DDEBUG $^ $(LIBS) -o test

clean:
	rm -rf $(BUILD_DIR)
	rm test

.PHONY: all clean