:
# Makefile for igx
#
PROJECT_ROOT_PATH = ../../
include $(PROJECT_ROOT_PATH)/MakeRules

SRCS = $(wildcard *.c) 
SRCS_CC = $(wildcard *.cc)
OBJS = $(SRCS:.c=.o) $(SRCS_CC:.cc=.o)

all: clean $(OBJS)

.cc.o: 
	$(CXX) $(CXXFLAGS) $<
.c.o: 
	$(CC) $(CFLAGS) $<

# for unit test.
# $ make CFLAGS="-I../ -D__UNIT_TEST__ -c" unittest
#unittest:
#	$(CXX) -o unittest igx_thread_ex.cc -I../ -DDEBUG -D__UNIT_TEST__ $(PROJECT_ROOT_PATH)/libs/libigx.a -lpthread -lc
install:
	mv *.o $(PROJECT_ROOT_PATH)/objs
clean:
	rm -f *.o
	rm -f unittest
