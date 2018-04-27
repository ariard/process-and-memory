EXTRA_CFLAGS		= -I$(src)/include/
PWD			= $(shell pwd)
SRCS			= pid_info/
VERSION			= `uname -r`
KDIR			= /usr/src/linux-$(VERSION)/

default:
	cp kernel.Makefile $(KDIR)kernel/Makefile
	cp syscalls.h $(KDIR)include/linux/syscalls.h
	cp syscall_64.tbl $(KDIR)arch/x86/entry/syscalls/syscall_64.tbl
	cp $(SRCS)/*.c $(KDIR)kernel/
	cp /boot/config-$(VERSION) $(KDIR).config; \
	$(MAKE) -j4 -C $(KDIR) 1>/tmp/log 2>/tmp/err_log
	cp $(KDIR)arch/x86/boot/bzImage /boot/vmlinuz-$(VERSION)
	reboot

continue:
	$(MAKE) -j4 -C $(KDIR)
	cp $(KDIR)arch/x86/boot/bzImage /boot/vmlinuz-$(VERSION)
	reboot

clean:
	$(MAKE) clean -C $(KDIR)
	rm -f $(KDIR)kernel/get_pid_info.c

re: clean default

