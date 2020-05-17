COMMON_CFLAGS=-Wall
export COMMON_CFLAGS

.PHONY: all
all: debug release

.PHONY: debug
debug:
	$(MAKE) -f makefile.debug

.PHONY: release
release:
	$(MAKE) -f makefile.release

.PHONY: clean-dbg
clean-dbg:
	$(MAKE) -f makefile.debug clean

.PHONY: clean-rel
clean-rel:
	$(MAKE) -f makefile.release clean

.PHONY: clean
clean:
	-rm -rf ./build

.PHONY: run-dbg
run-dbg:
	./build/debug/bin/tic-tac-toe

.PHONY: run-rel
run-rel:
	./build/release/bin/tic-tac-toe
