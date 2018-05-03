EXTRA_CFLAGS		= -I$(src)/include/
PWD			= $(shell pwd)
SRCS			= pid_info/
VERSION			= 4.16.0
KDIR			= /usr/src/linux-$(VERSION)/

cp:
	cp kernel.Makefile $(KDIR)kernel/Makefile
	cp syscalls.h $(KDIR)include/linux/syscalls.h
	cp syscall_64.tbl $(KDIR)arch/x86/entry/syscalls/syscall_64.tbl
	cp /boot/config-$(VERSION) $(KDIR).config; \

config:
	make olddefconfig -C $(KDIR)

default: cp config partial

partial:
	cp $(SRCS)/get_pid_info.c $(KDIR)kernel/
	$(MAKE) -j4 -C $(KDIR) 2>/tmp/err_log 1>/tmp/log
	cp $(KDIR)arch/x86/boot/bzImage /boot/vmlinuz-$(VERSION)
	reboot

clean:
	$(MAKE) clean -C $(KDIR)
	rm -f $(KDIR)kernel/get_pid_info.c

re: clean default
