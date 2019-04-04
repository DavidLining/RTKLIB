#
# make all cui applications by gcc
#

BINDIR = /usr/local/bin
DIRS   = rtkrcv

export CC = $(CURDIR)/../../prebuilts/toolschain/usr/bin/arm-linux-gcc

all:
	@for i in $(DIRS); do $(MAKE) -C $$i/gcc; done

install:
	@echo installing...
	@for i in $(DIRS); do cp $$i/gcc/$$i $(BINDIR); done

clean:
	@echo cleaning up...
	@for i in $(DIRS); do $(MAKE) -C $$i/gcc clean; done

