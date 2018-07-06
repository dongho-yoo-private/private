# Makefile
#
#

TARGET:=test
CC:=g++
LDFLAGS:= -lc -lpthread
SRCS:=$(wildcard *.c) $(wildcard *.cc) $(wildcard private/*.c) $(wildcard private/*.cc)
OBJS:=$(SRCS:%.cc=%.o) $(SCRS:%.c=%.o)

all: clean $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
clean:
	rm -f $(OBJS) $(TARGET)
