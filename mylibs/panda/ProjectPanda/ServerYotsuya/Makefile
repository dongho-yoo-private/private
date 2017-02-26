
YOTSUYA_HOME=/usr/local/goyotsuya

all: user install 
#proxy

clean:
	make -f Makefile.user clean
#	make -f Makefile.proxy clean
	make -C base clean

svrybase:
	make -C base

user: svrybase
	make -f Makefile.user

#proxy: svrybase
#	make -f Makefile.proxy

install: user
	cp -f *.h $(YOTSUYA_HOME)/include/svry

