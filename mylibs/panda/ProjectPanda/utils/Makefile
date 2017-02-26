#
#
#
#
OBJS = 	yiList.o yiQueue.o yiStack.o yithread.o yimutex.o yitrace.o yitime.o yifile.o yifile.posix.o \
       	yialloc.o yiEncoder.o yiHashFunc.o yisock.o yiencodex.o  yiupnp.o yiHashTable.o yistr.o \
	yipsr.o yiProfile.o yiSmtp.o yiListx.o yipop3.o yiUtil.o

# remove -fPIC
#CFLAGS = -O2 -D "__POSIX__" -D "__use_sse2__" -D "__DEBUG__" -D"__OSX__" -D"_x64" -c

# for debug
CFLAGS = -g -D "__POSIX__" -D "__use_sse2__" -D "__DEBUG__" -D"__OSX__" -D"_x64" -c
#CFLAGS = -g -O2 -D "__POSIX__" -D "__use_sse2__" -c
CXX = g++
CC = gcc
TARGET = libyi.a
YOTSUYA_HOME=/usr/local/goyotsuya

all: clean $(TARGET) install

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf objs
.cpp.o:
	$(CXX) $(CFLAGS) $<

.c.o:
	$(CC) $(CFLAGS) $<

$(TARGET):$(OBJS)
	ar -r $@ $(OBJS) 

install:
	cp -f $(TARGET) $(YOTSUYA_HOME)/libs
	cp -f *.h $(YOTSUYA_HOME)/include
	mkdir objs
	mv -f *.o objs
