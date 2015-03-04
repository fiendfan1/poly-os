RUSTC=rustc
CARGO=cargo
NASM=nasm
# Rust uses different name for platform
RUSTARCH=i686-unknown-linux-gnu
RUSTREPO := ../rust-src

HOST?=$(shell ../default-host.sh)
HOSTARCH:=$(shell ../target-triplet-to-arch.sh $(HOST))

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -fbuiltin -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_myos_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR:=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

### HASKELL ###

JHC:=jhc

kernel/haskell/kernel.c: kernel/haskell/Kernel.hs
	$(JHC) -fffi -C $< --tdir kernel/haskell/rts -o $@

HS_CFLAGS:=-Ikernel/haskell/rts

#HS_RTS_CFILE:=$(wildcard kernel/haskell/rts/)

#kernel/kernel.o: kernel/haskell/kernel.c
#	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS) $(HS_CFLAGS)

## END HASKELL ##

### IDRIS ###

#XXX IDRIS:=/home/fiendfan1/workspace/idris/test/.cabal-sandbox/bin/idris
IDRIS := idris
IDRIS_RTS_DIR := $(HOME)/.cabal/share/x86_64-linux-ghc-7.8.4/idris-0.9.16/rts

#XXX IDRIS_RTS_CFILES:=$(wildcard kernel/idris/idris_rts/*.c)
#IDRIS_RTS_CFILES := $(shell find $(IDRIS_RTS_DIR) -maxdepth 1 -name '*.c')
IDRIS_RTS_OBJS_NAMES :=\
idris_gmp.o idris_opts.o idris_stdfgn.o idris_bitstring.o \
idris_heap.o idris_rts.o idris_gc.o idris_stats.o mini-gmp.o
IDRIS_RTS_OBJS := $(IDRIS_RTS_OBJS_NAMES:%.o=$(IDRIS_RTS_DIR)/%.o)

#IDRIS_CFLAGS=-Ikernel -Lkernel/idris_rts -lidris_rts -Ikernel/idris_rts -Ikernel/prelude -Ikernel/base
#XXX IDRIS_CFLAGS:=-DIDRIS_TARGET_TRIPLE=$(HOSTARCH)
IDRIS_CFLAGS:=-DIDRIS_TARGET_TRIPLE=$(HOSTARCH) -I$(IDRIS_RTS_DIR) -L$(IDRIS_RTS_DIR)

kernel/idris/idrk.c: kernel/idris/Kernel.idr
	$(IDRIS) -S $< -o $@
	cat $(IDRIS_RTS_DIR)/idris_main.c >> $@
#XXX	cat kernel/idris/idris_rts/idris_main.c.unused >> $@
	# #include <idris_rts.h> -> #include <idris/idris_rts.h>
#XXX	sed -i -E 's/#include <idris_(.+)>/#include <idris\/idris_\1>/g' $@
	# #include "idris_rts.h" -> #include <idris/idris_rts.h>
#XXX	sed -i -E 's/#include \"(.+)\"/#include <idris\/\1>/g' $@

kernel/kernel.o: kernel/idris/idrk.c $(IDRIS_RTS_OBJS)
	$(CC) -c kernel/idris/idrk.c -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS) $(IDRIS_CFLAGS)

## END IDRIS ##

OBJS:=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
$(IDRIS_RTS_OBJS) \

CRTI_OBJ:=$(ARCHDIR)/crti.o
CRTBEGIN_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ:=$(shell $(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=crtend.o)
CRTN_OBJ:=$(ARCHDIR)/crtn.o

ALL_OUR_OBJS:=\
$(CRTI_OBJ) \
$(OBJS) \
$(CRTN_OBJ) \

OBJ_LINK_LIST:=\
$(CRTI_OBJ) \
$(CRTBEGIN_OBJ) \
$(OBJS) \
$(CRTEND_OBJ) \
$(CRTN_OBJ)

all: myos.kernel

.PHONY: all clean install install-headers install-kernel

myos.kernel: $(OBJ_LINK_LIST) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(OBJ_LINK_LIST) $(LDFLAGS) $(LIBS)

### RUST ###

RCFLAGS := -O --target $(RUSTARCH) -L kernel -C no-stack-check -Z no-landing-pads -Z no-verify

rust_src:
	wget -O rust_src.zip https://github.com/rust-lang/rust/archive/1.0.0-alpha.2.zip
	unzip rust_src.zip
	mv rust-1.0.0-alpha.2 $(RUSTREPO)
	rm rust_src.zip

## RUST LIBS

LIBCORE := $(RUSTREPO)/src/libcore/libcore.rlib

$(LIBCORE): $(RUSTREPO)/src/libcore/lib.rs
	$(RUSTC) $(RCFLAGS) -o $@ --crate-type=rlib $<


kernel/libmorestack.a: $(RUSTREPO)/src/rt/arch/$(HOSTARCH)/morestack.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -D__linux__ -o $@.o -c $^
	$(AR) cru $@ $@.o
	rm -f $@.o

kernel/libcompiler-rt.a:
	ln -sf $(shell $(CC) $(CFLAGS) $(CPPFLAGS) -print-file-name=libgcc.a) $@

## RUST KERNEL

#kernel/kernel.o: kernel/kernel.rs $(LIBCORE)
#	$(RUSTC) $(RCFLAGS) --crate-type staticlib -o $@ --emit obj $< --extern core=$(LIBCORE)


%.o: %.rs $(LIBCORE) kernel/libmorestack.a kernel/libcompiler-rt.a
#	$(RUSTC) $(RCFLAGS) --crate-type staticlib -o $@ $< --extern core=$(LIBCORE)
	$(RUSTC) $(RCFLAGS) --crate-type staticlib -o $@.tmp $< --extern core=$(LIBCORE)
	mv $@.tmp $@
	#$(AR) t $@.a | grep .bytecode | xargs -n 1 $(AR) dv $@.a
#	$(RUSTC) $(RCFLAGS) --crate-type staticlib -o $@ --emit obj $< --extern core=$(LIBCORE)


### END RUST ###

%.o: %.c
	$(CC) -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS) $(IDRIS_CFLAGS)

%.o: %.S
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

%.o: %.asm
	$(NASM) -f elf32 $< -o $@

clean:
	rm -f myos.kernel $(OBJS) $(ALL_OUR_OBJS) kernel/idris/idrk.c kernel/idris/idris_rts/*.o *.o */*.o */*/*.o *.ibc */*.ibc */*/*.ibc *.a */*.a */*/*.a

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -RTv include $(DESTDIR)$(INCLUDEDIR)

install-kernel: myos.kernel
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp myos.kernel $(DESTDIR)$(BOOTDIR)
