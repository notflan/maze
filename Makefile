SOURCE:= src/*.c src/tiles/*.c
INCLUDE:= include/
CFLAGS:= -Wall -pedantic -g -std=gnu11
LFLAGS:= -lncurses -lrt
BUILD:= build

maze:
	gcc $(SOURCE) $(CFLAGS) -I$(INCLUDE) -o $(BUILD)/$@ $(LFLAGS)
	./$(BUILD)/$@

clean:
	rm -f $(BUILD)/*
