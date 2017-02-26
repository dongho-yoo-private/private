#
# svrybase Makefile (Last updated 2009. 11. 11.)
#
#

OBJS = 	register_test.o 

CFLAGS = -g -D "__POSIX__" -O2 -c
CC = g++
TARGET = register_test
LDFLAGS = ../base/libsvrybase.a $(YOTSUYA_HOME)/libs/libyi.a -lpthread -lstdc++ -lc
INCLUDES= -I $(YOTSUYA_HOME)/include

all: clean $(TARGET) install

clean:
	rm -f $(OBJS) $(TARGET)

.cpp.o:
	$(CC) $(INCLUDES) $(CFLAGS) $<

$(TARGET):$(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

install:
	cp -f $(TARGET) $(YOTSUYA_HOME)/libs
	mv -f $(OBJS) objs
