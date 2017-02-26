#
# ssvr Makefile (Last updated 2009. 11. 11.)
# it is sample server.
#
#

OBJS = cmdecho.o cmdq.o

CFLAGS = -g -O2 -D "__POSIX__" -c
CC = g++
TARGET = cmdecho
TARGET2 = cmdq
LDFLAGS = $(YOTSUYA_HOME)/libs/libsvrybase.a $(YOTSUYA_HOME)/libs/libyi.a -lpthread -lstdc++ -lc

INCLUDES= -I $(YOTSUYA_HOME)/include 

all: clean $(TARGET) $(TARGET2)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(TARGET).o $(LDFLAGS)

$(TARGET2): $(OBJS)
	$(CC) -o $@ $(TARGET2).o $(LDFLAGS)

.cpp.o:
	$(CC) $(INCLUDES) $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET2)

install:
	cp -f $(TARGET) $(TARGET2) $(YOTSUYA_HOME)/bin
