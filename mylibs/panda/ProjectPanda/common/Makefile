#
#
#
#
OBJS = 	sycenv.o sycHashL3.o sycHashL2.o

CFLAGS = -g -D "__POSIX__" -D "__use_sse2__" -D "__YI_DEBUG_ON__" -c -Wall
CXX = g++
CC = gcc
LINKER = $(CC)
INCLUDES = -I $(YOTSUYA_HOME)/include
SHAREDLIBFLAGS= -shared -W1,-soname,libyic

TARGET = libsyc.a

all: clean $(OBJS) $(TARGET) install

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf objs
.cpp.o:
	$(CXX) $(INCLUDES) $(CFLAGS) $<

.c.o:
	$(CC) $(INCLUDES) $(CFLAGS) $<

$(TARGET) : $(OBJS)
	ar -r $@ $(OBJS) 

install:
	cp -f $(TARGET) $(YOTSUYA_HOME)/libs
	cp -f *.h $(YOTSUYA_HOME)/include
	mkdir objs
	mv *.o objs
