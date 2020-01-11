GXX = gcc
OUT_NAME = k-storager
DEPS_PATH = deps/libuv/.libs/libuv.a
SRC_PATH = src/hashmap.c src/cJSON.c src/store.c src/server.c src/k_storager.c

install: install-deps
	@cp -r ./k-storager /usr/local/bin/k-storager

build: clean
	@$(GXX) $(DEPS_PATH) $(SRC_PATH) -o $(OUT_NAME)

install-deps:
	@sh deps/libuv/autogen.sh
	@cd deps/libuv && ./configure
	@cd deps/libuv && make
	@cd deps/libuv && make install

clean:
	@echo "clean ..."
	@rm -rf ./out/*

.PHONY: build install-deps install