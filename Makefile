ARCH = `uname`_`uname -p`
#GCC_VER = _gcc3

all: gvar

gvar:
	cd makefiles/$(ARCH); make -f Makefile$(GCC_VER);

clean:
	cd makefiles/$(ARCH); make -f Makefile$(GCC_VER) clean


