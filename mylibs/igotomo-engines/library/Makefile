:
# Makefile for igx
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
clean:
	rm -f *.o
