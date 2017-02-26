#
#
#
#
CXX = g++
CC = gcc
LINKER = $(CXX)
INCLUDES = -I $(YOTSUYA_HOME)/include
SHAREDLIBFLAGS= -shared -W1,-soname,libyotsuya.so

LIBPATH=$(YOTSUYA_HOME)/libs
TARGET = libyotsuya.so
#TARGETSO = libyotsuya.so

LIBS = $(LIBPATH)/libyi.a $(LIBPATH)/libsy.a $(LIBPATH)/libsyc.a

all: clean util svr cmm $(TARGET) install

makeenv:
	rm -rf $(YOTSUYA_HOME)/*
	mkdir $(YOTSUYA_HOME)/libs
	mkdir $(YOTSUYA_HOME)/include
	mkdir $(YOTSUYA_HOME)/include/svry
	mkdir $(YOTSUYA_HOME)/bin

clean:
	rm -f $(LIBS) $(TARGET) $(TARGETSO)

util:
	make -C utils	

svr:
	make -C ServerYotsuya

cmm:
	make -C common


$(TARGET) : $(LIBS)
	$(LINKER) $(SHAREDLIBFLAGS) -o $@ $(LIBS) 

install:
	cp -f $(TARGET) $(YOTSUYA_HOME)/libs
#	ln -s 	$(YOTSUYA_HOME)/libs/$(TARGET) $(YOTSUYA_HOME)/libs/$(TARGETSO)
