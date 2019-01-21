include /home/homeuser/hwdb/kernel/Modules.mk

default: $(build_links)

all: default

check: all

clean:
	rm -f *.o *.ko *.mod.* Module.symvers Module.order
	for d in $(build_links); do if test -h $$d; then rm $$d; fi; done

KOBJ := /lib/modules/$(shell uname -r)/build

ifneq ($(KERNELRELEASE),)
	obj-m := klogger.o
	obj-y := hashtable.o hwdbflow.o
	klogger-objs := hashtable.o hwdbflow.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	depmod -a

# clean:
# $(MAKE) -C $(KERNELDIR) M=$(PWD) clean

endif
