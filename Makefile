CC=gcc

CFLAGS = -Wall -Wextra -std=c99 -g

LIBS = -lm

BUILD_DIR = build
OUTPUT_DIR = .

BUILD_FLAGS = -DABORT_ON_OVERSAMPLE #-DDEBUG_PRINT

SRC_FILES = test.c gw_knuth.c sample_wor.c

all: test

test: $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(BUILD_FLAGS) $^ $(LIBS) -o test

test_debug: $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(BUILD_FLAGS) $^ $(LIBS) -o test

clean:
	rm -rf $(BUILD_DIR)
	rm test

.PHONY: all clean