#
# Makefile for IGX root.
# http://minus9d.hatenablog.com/entry/20140203/1391436293
#
SUBDIRS = data_struct debug io library thread go network

all: subdirs install libigx 

subdirs: 
	for dir in $(SUBDIRS); do \
    	make -C $$dir; \
	done

libigx:
	ar cr libigx.a objs/*.o
	mv libigx.a ./libs/

clean:
	for dir in $(SUBDIRS); do \
    	make -C $$dir clean; \
	done
	rm -f objs/*.o
	rm -f libigx.a
install:
	for dir in $(SUBDIRS); do \
    	make -C $$dir install; \
	done
