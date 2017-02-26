#
# Makefile for python Wrapper
#
PROJECT_ROOT_PATH = ..
include $(PROJECT_ROOT_PATH)/MakeRules

SRCS = $(wildcard *.c) 
SRCS_CC = $(wildcard *.cc)
OBJS = $(SRCS:.c=.o) $(SRCS_CC:.cc=.o)

all: clean $(OBJS)

.cc.o: 
	$(CXX) $(CXXFLAGS) $<
.c.o: 
	$(CC) $(CFLAGS) $<

install:
	mv *.o $(PROJECT_ROOT_PATH)/objs
encrypt:
	$(CC) -o igx_encrypt_test igx_encrypt.c $(PROJECT_ROOT_PATH)/debug/igx_log.c igx_checksum.c -D__UNIT_TEST__ $(CFLAGS)
clean:
	rm -f *.o
