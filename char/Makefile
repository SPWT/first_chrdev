#!/bin/bash

CROSS_COMPILE:=arm-linux-gnueabi-
ARCH:=arm
CC:=$(CROSS_COMPILE)gcc
LD:=$(CROSS_COMPILE)ld

obj-m += module_chrdev.o

KDIR := /home/baohua/Downloads/kernel/linux-4.1.38
#KDIR := /home/baohua/Downloads/kernel/linux-3.16

PWD ?= $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules ARCH=$(ARCH)

clean:
	rm -rf *.o *.ko *.mod *.symvers
