EXTRA_CFLAGS		= -I$(src)/include/
PWD			= $(shell pwd)
SRCS			= pid_info/
VERSION			= `uname -r`
KDIR			= /usr/src/linux-$(VERSION)/

default:
	$(MAKE) clean $(KDIR)
	cp kernel.Makefile $(KDIR)Makefile
	cp syscalls.h $(KDIR)include/linux/syscalls.h
	cp syscall_64.tbl $(KDIR)arch/x86/entry/syscalls/syscall_64.tbl
	rm -rf $(KDIR)/pid_info && cp -r $(SRCS) $(KDIR)
	if [ -a .config ] ; \
	then \
		echo ".config OK"; \
	else \
		cp /boot/config-$(VERSION) $(KDIR).config; \
	fi;
	$(MAKE) -j4 -C $(KDIR) 
	cp $(KDIR)arch/x86/boot/bzImage /boot/vmlinuz-$(VERSION)
	reboot

continue:
	$(MAKE) -j4 -C $(KDIR)
	cp $(KDIR)arch/x86/boot/bzImage /boot/vmlinuz-$(VERSION)
	reboot

clean:
	@rm -f Module.symvers $(NAME).mod.c $(NAME).mod.o $(NAME).o modules.order
	@rm -f built-in.o

fclean: clean
	@rm -f $(NAME).ko
	@rm -f $(SRCS)*.o

re: clean default

