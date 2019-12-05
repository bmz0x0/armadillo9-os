
ARCH				= arm
CROSS_COMPILE		= arm-elf-

AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump


cc-option = $(shell if $(CC) $(CFLAGS) $(1) -S -o /dev/null -xc /dev/null \
             > /dev/null 2>&1; then echo "$(1)"; else echo "$(2)"; fi ;)

CFLAGS 		:= -Wall -Wstrict-prototypes -Wno-trigraphs \
	  	   -fno-strict-aliasing -fno-common \
		   -ffreestanding
CFLAGS		+= -O
CFLAGS		+= -g

# arch/arm/Makefile
CFLAGS		+=-fno-omit-frame-pointer -mapcs -mno-sched-prolog
CFLAGS		+= -nostdinc -mlittle-endian
CFLAGS		+= -isystem $(shell $(CC) -print-file-name=include)
CFLAGS		+= -Iinclude

# CPU arm920T
CFLAGS_ABI	:=$(call cc-option,-mapcs-32,-mabi=apcs-gnu)
CFLAGS		+=$(CFLAGS_ABI) $(call cc-option,-mshort-load-bytes,$(call cc-option,-malignment-traps,)) -msoft-float -Uarm

CFLAGS += -march=armv4 -mtune=arm9tdmi

# warn about C99 declaration after statement
CFLAGS += $(call cc-option,-Wdeclaration-after-statement,)

# disable pointer signedness warnings in gcc 4.0
CFLAGS += $(call cc-option,-Wno-pointer-sign,)


AFLAGS		:= -D__ASSEMBLY__
AFLAGS		+=$(CFLAGS_ABI) -msoft-float
AFLAGS		+= -march=armv4 -mtune=arm9tdmi
AFLAGS		+= -nostdinc -mlittle-endian
AFLAGS		+= -Iinclude


#LDFLAGS		:= --no-undefined -X -T kernel.lds
LDFLAGS		:=
LDFLAGS += -EL -p --no-undefined -X


#INC := $(wildcard *.h)
#FIND_SRCS = $(wildcard *.c *.S)
FIND_OBJS = $(filter-out %head.o,$(subst .S,.o,$(wildcard $(d)/*.S))) \
			$(subst .c,.o,$(wildcard $(d)/*.c))

FIND_DEP_FILES = $(wildcard $(d)/.*.d)

#DIRS
srctree = 
dep_files :=
SUBDIRS = init mm lib driver

#include $(patsubst %,%/Makefile,$(SUBDIRS))

OBJS := $(foreach d, $(SUBDIRS), $(FIND_OBJS))
dep_files := $(foreach d, $(SUBDIRS), $(FIND_DEP_FILES))

.PHONY: all
all: Image


Image: kernel
	$(OBJCOPY) -O binary -S $< $@


kernel: kernel.lds init/head.o $(OBJS)
	$(LD) $(LDFLAGS) -o $@ -T $^
	$(NM) -n kernel > System.map

init/head.o: init/head.S
	$(CC) -Wp,-MD,$(dir $@)$(patsubst %.o,.%.d,$(notdir $@)),-MT,$@ $(AFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f init/head.o
	rm -f $(dep_files)
	rm -f *.dump
	rm -f System.map
	rm -f kernel
	rm -f Image



%.o: %.S
	$(CC) -Wp,-MD,$(dir $@)$(patsubst %.o,.%.d,$(notdir $@)),-MT,$@ $(AFLAGS) -c -o $@ $<
#	$(CC) $(AFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) -Wp,-MD,$(dir $@)$(patsubst %.o,.%.d,$(notdir $@)),-MT,$@ $(CFLAGS) -c -o $@ $<
#	$(CC) $(CFLAGS) -c -o $@ $<


ifneq ($(dep_files),)
  #$(dep_files): ;	# Do not try to update included dependency files
  include $(dep_files)
endif


