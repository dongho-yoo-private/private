#
# Makefile for igx
#
PROJECT_ROOT_PATH = ..
include $(PROJECT_ROOT_PATH)/MakeRules

CC=gcc
SRCS = $(wildcard *.c) 
SRCS_CC = $(wildcard *.cc)
OBJS = $(SRCS:.c=.o) $(SRCS_CC:.cc=.o)

all: clean root sample igx_socket igxhttp $(OBJS)

.cc.o: 
	$(CXX) $(CXXFLAGS) $<
.c.o: 
	$(CC) $(CFLAGS) $<
root:
	make -C ../
sample:
	$(CXX) -o go_engine_sample go_engine_sample.cc -I../ -DDEBUG $(PROJECT_ROOT_PATH)/libs/libigx.a -lpthread -lc -liconv
igx_socket:
	$(CC) -o igxsocket igx_socket.c -I../ -DDEBUG $(PROJECT_ROOT_PATH)/libs/libigx.a -lpthread -lc -lssl -lcrypto -lc++ -liconv
igxhttp: 
	$(CC) -o igxhttp igxhttp.c -I../ -DDEBUG $(PROJECT_ROOT_PATH)/libs/libigx.a -lpthread -lc -lssl -lcrypto -lc++ -liconv

install:
	mv *.o $(PROJECT_ROOT_PATH)/objs
clean:
	rm -f *.o go_engine_sample igxsocket igxhttp
	make -C ../ clean 
