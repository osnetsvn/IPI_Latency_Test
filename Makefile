obj-m += ipi_latency.o
ccflags-y := -std=gnu99 -Wno-declaration-after-statement

all: modules

modules:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

insert:
	insmod ipi_latency.ko

remove:
	rmmod ipi_latency

.PHONY: clean
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
