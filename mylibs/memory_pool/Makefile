# Makefile
#
#

TARGET:=test
CC:=gcc
CXX:=g++
LDFLAGS:= -lc -lpthread
SRCS := $(wildcard ./*.c) $(wildcard ./private/*.c) 
OBJS := $(SRCS:%.c=%.o) 
SRCS_CC:=$(wildcard ./*.cc) $(wildcard ./private/*.cc)
OBJS_CC:=$(SRCS_CC:%.cc=%.o)

all: clean print $(TARGET)

$(TARGET): $(OBJS) $(OBJS_CC)
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
print:
	@echo $(OBJS)
	@echo $(OBJS_CC)
clean:
	find ./ -name "*.o" -exec rm -f {} \;
	rm -f $(TARGET)
