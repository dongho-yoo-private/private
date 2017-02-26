#
# preregtister Makefile (Last updated 2009. 11. 11.)
# it is sample server.
#
#

OBJS = makeres.o resource.o

CFLAGS = -g -O2 -D "__POSIX__" -c
CC = gcc
TARGET = makeres
LDFLAGS = $(YOTSUYA_HOME)/libs/libyi.a -lc

INCLUDES= -I $(YOTSUYA_HOME)/include 

all: clean $(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(TARGET)

