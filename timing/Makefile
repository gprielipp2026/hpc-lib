CC=gcc
CFLAGS=-Wall

SRCS=$(wildcard *.c)
OBJS=$(SRCS:%.c=%.o)

TGT=libtiming.a

all: $(TGT)

$(TGT): $(OBJS)
	ar rcs $@ $<

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(RM) $(TGTS) $(OBJS)

.phony: clean all
