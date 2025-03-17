CC = clang++
DBG_BIN = lldb
# CFLAGS = -D_GNU_SOURCE
CFLAGS += -std=c++17
CFLAGS += -Wall -pedantic -Werror -Wextra -Wmissing-declarations
CFLAGS += -L/usr/local/lib
LDFLAGS = -I/user/local/include -lraylib -lm -lpthread -ldl 
ASANFLAGS = -fsanitize=address -fno-common -fno-omit-frame-pointer

SRC_FILES = ./src/*.cpp
BIN_DIR = ./bin
BIN = $(BIN_DIR)/sand-sim

build: bin-dir
	$(CC) $(CFLAGS) $(LIBS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

build-asan:
	$(CC) $(ASANFLAGS) $(CFLAGS) $(LIBS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

bin-dir:
	mkdir -p $(BIN_DIR)

debug: debug-build
	# LSAN_OPTIONS=verbosity=1:log_threads=1
	$(DBG_BIN) $(BIN) $(ARGS)

debug-build: bin-dir
	$(CC) $(ASANFLAGS) $(CFLAGS) -g $(LIBS) $(SRC_FILES) -o $(BIN) $(LDFLAGS)

run: build
	@$(BIN) $(ARGS)

run-hud: build
	LD_PRELOAD=/usr/lib/mangohud/libMangoHud_dlsym.so mangohud $(BIN) $(ARGS)

clean:
	rm -rf $(BIN_DIR)/* 

gen-compilation-db:
	bear -- make build
