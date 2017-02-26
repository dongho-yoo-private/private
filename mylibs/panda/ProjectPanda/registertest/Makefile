#
# ssvr Makefile (Last updated 2009. 11. 11.)
# it is sample server.
#
#

OBJS = registertest.o 

CFLAGS = -g -O2 -D "__POSIX__" -c
CC = g++
TARGET =registertest 
LDFLAGS = ../common/libsyc.a ../utils/libyi.a -lpthread -lstdc++ -lc

INCLUDES= -I $(YOTSUYA_HOME)/include 

all: clean $(TARGET) 

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CC) $(INCLUDES) $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(TARGET) 

