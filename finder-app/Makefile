#C Compiler
CC = gcc
#if CROSS_COMPILE is defined use it as prefix
 ifdef CROSS_COMPILE 
	CC = $(CROSS_COMPILE)gcc
 endif

#define source and output
SRC=writer.c 
OUT=writer
all :$(OUT)
#define rule tocompile 
$(OUT): $(SRC)
	$(CC) -o $@ $<

.PHONY: clean
clean: 
	rm -f $(OUT) *.oA