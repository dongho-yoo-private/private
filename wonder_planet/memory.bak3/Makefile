# Makefile
#
#

TARGET:=test
CC:=gcc
CXX:=g++
LDFLAGS:= -lc -lpthread
SRCS:=$(wildcard ./*.c) $(wildcard ./private/*.c) $(wildcard ./*.cc) $(wildcard ./private/*.cc)
OBJS:=$(SRCS:%.c=%.o) $(SRCS:%.cc=%.o)

all: clean print $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
print:
	@echo $(SRCS)
clean:
	find ./ -name "*.o" -exec rm -f {} \;
    rm -f $(TARGET)
