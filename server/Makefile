#C Compiler
CC = gcc	
CFLAGS = -g -Wall -Werror -lm
#if CROSS_COMPILE is defined use it as prefix
 ifdef CROSS_COMPILE 
	CC = $(CROSS_COMPILE)gcc
 endif

#define source and output
SRC=aesdsocket.c 
OUT=aesdsocket
all :$(OUT)
#define rule tocompile 
$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean: 
	rm -f $(OUT) *.o 