SOURCE:= src/*.c src/tiles/*.c
INCLUDE:= include/
CFLAGS?= -gz -ggdb
CFLAGS+= -Wall -pedantic -std=gnu11 -fgraphite -fno-strict-aliasing
LFLAGS+= -lncurses -lrt -fuse-linker-plugin
BUILD:= build
STRIP=strip

maze:
	gcc $(SOURCE) $(CFLAGS) -I$(INCLUDE) -o $(BUILD)/$@ $(LFLAGS)
	$(STRIP) ./$(BUILD)/$@
	./$(BUILD)/$@

clean:
	rm -f $(BUILD)/*
