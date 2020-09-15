obj-m = push_switch_driver.o led_driver.o fnd_driver.o dot_matrix_driver.o

PWD = $(shell pwd)

all: driver app

driver:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

app:
	gcc -Wall -static -o test1 test1.c

clean:
	rm -rf *.ko *.mod.* *.o test1 Module.symvers modules.order .test1* .tmp*
