obj-m	:= ubuntu_kernel_module_8.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD       := $(shell pwd)

ccflags-y := -DDEBUG -g -std=gnu99 -Werror -Wno-declaration-after-statement -Wframe-larger-than=1000000000

.PHONY: all clean

all: default

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod *.mod.c *.mod.o *.cmd *.order *.symvers
