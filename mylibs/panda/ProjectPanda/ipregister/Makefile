#
# ssvr Makefile (Last updated 2009. 11. 11.)
# it is sample server.
#
#

OBJS = ipregister.o 
YOTSUYA_HOME=/usr/local/goyotsuya

CFLAGS = -g -O2 -D "__POSIX__" -c
CC = g++
TARGET = ipregister
LDFLAGS = $(YOTSUYA_HOME)/libs/libyi.a -lpthread -lstdc++ -lc

INCLUDES= -I $(YOTSUYA_HOME)/include 


all: clean $(TARGET) $(TARGET2)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(TARGET).o $(LDFLAGS)

.cpp.o:
	$(CC) $(INCLUDES) $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(TARGET)

install:
	cp -f $(TARGET) $(YOTSUYA_HOME)/bin
