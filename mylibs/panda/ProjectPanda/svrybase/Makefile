#
# svrybase Makefile (Last updated 2009. 11. 11.)
#
#

OBJS = 	svrybase.init.o svrybase.create.o svrybase.loop.o svrybase.dispatch.o \
	svrybase.virtuals.o svrybase.operations.o ConnectionManager.o ConnectionInfo.o 

CFLAGS = -g -D "__POSIX__" -O2 -c
CC = g++
TARGET = libsvrybase.a
INCLUDES= -I $(YOTSUYA_HOME)/include

all: clean $(TARGET) install

clean:
	rm -f $(OBJS) $(TARGET)

.cpp.o:
	$(CC) $(INCLUDES) $(CFLAGS) $<

$(TARGET):$(OBJS)
	ar -r $@ $(OBJS) 

install:
	cp -f $(TARGET) $(YOTSUYA_HOME)/libs
	cp -f *.h $(YOTSUYA_HOME)/include
	mv -f $(OBJS) objs
