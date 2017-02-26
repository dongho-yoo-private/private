#
#
#
#
OBJS = 	sycgibase.o

# remove -fPIC
CFLAGS = -I $(YOTSUYA_HOME)/include -D "__POSIX__" -D "__use_sse2__" -D "__YI_DEBUG_ON__" -c
CXX = g++
CC = gcc
TARGET = libcgibase.a

all: clean $(TARGET) install

clean:
	rm -f $(OBJS) $(TARGET)
.cpp.o:
	$(CXX) $(CFLAGS) $<

.c.o:
	$(CC) $(CFLAGS) $<

$(TARGET):$(OBJS)
	ar -r $@ $(OBJS) 

install:
	cp -f $(TARGET) $(YOTSUYA_HOME)/libs
	cp -f *.h $(YOTSUYA_HOME)/include
	mv -f *.o objs
