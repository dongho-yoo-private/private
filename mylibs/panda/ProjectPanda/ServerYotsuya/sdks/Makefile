#
# svrybase Makefile (Last updated 2009. 11. 11.)
#
#

OBJS = 	uthp.o ypn.o ypnGoNetwork.o

CFLAGS = -g -D "__POSIX__" -O2 -c
CC = g++
TARGET = libsvrysdk.a
INCLUDES= -I $(YOTSUYA_HOME)/include

all: clean $(TARGET) test1 test_connect

clean:
	rm -f $(OBJS) $(TARGET)

.cpp.o:
	$(CC) $(INCLUDES) $(CFLAGS) $<

$(TARGET):$(OBJS)
	ar -r $@ $(OBJS) 

test1:$(TARGET)
	make -f Makefile.test1

test_connect:$(TARGET)
	make -f Makefile.test_connect

install: 
	cp -f $(TARGET) $(YOTSUYA_HOME)/libs
	cp -f *.h $(YOTSUYA_HOME)/include
	mv -f $(OBJS) objs
