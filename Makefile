#Design by Rocky For Bare Smart210 no-os Programmer
#Date:	2014-12-29

#Define TARGET name for no-header binary filename
TARGET		:=	smart210_bare.bin
ELF_FILE	:=	$(TARGET:.bin=.elf)

#Define The Target Running-Enviroment
#In SD-Card, The base address is 0xD0020000
#In DRAM, The base address is 0x20000000
ENV		?=	SD

#Define Add checksum tools
TOOLS_HEADER	:= ./tools/mk210header
SUBDIR_TOOLS	:= ./tools

#Define OBJS for TARGET
OBJS	:= start.o
OBJS	+= smart210.o led_leaf.o
OBJS	+= uart.o lowlevel.o
OBJS	+= printf.o ctype.o
OBJS	+= _udivsi3.o _umodsi3.o
OBJS	+= clock.o

#Define COMPILER Flags
CFLAGS	+= -Wall -O2

CFLAGS	+= -D__DEBUG__
CFLAGS	+= -DCONFIG_SYS_PLL_ON
CFLAGS	+= -fno-builtin -nostdinc -I./inc
CFLAGS	+= -mabi=aapcs-linux 
CFLAGS	+= -I/opt/FriendlyARM/toolschain/4.5.1/lib/gcc/arm-none-linux-gnueabi/4.5.1/include

#CFLAGS += -DCONFIG_SYS_ICACHE_OFF

#Define LINKER Flags
LDFLAGS	+= -Tmap.lds
ifeq ($(ENV),SD)
LDFLAGS += -Ttext=0xD0020010
else
LDFLAGS += -Ttext=0x20000000
endif

#Define CROSS_COMPILER etc. info
CROSS_COMPILE	?= /opt/FriendlyARM/toolschain/4.5.1/bin/arm-linux-
CC				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY			:= $(CROSS_COMPILE)objcopy

#Rules
all:$(SUBDIR_TOOLS) $(TARGET)

.PHONY	: $(SUBDIR_TOOLS)
$(SUBDIR_TOOLS):
	$(MAKE) -C $(SUBDIR_TOOLS) all

$(TARGET):$(ELF_FILE) $(SUBDIR_TOOLS)
	$(OBJCOPY) -O binary $< $@.TMP
	$(TOOLS_HEADER) $@.TMP $@
	@cp -f $@ /mnt/hgfs/vmshare/$@
$(ELF_FILE):$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $<
%.o:%.S
	$(CC) -c $(CFLAGS) -o $@ $<

.PHONY : clean

clean:
	rm -f *.o $(TARGET) $(ELF_FILE) $(TOOLS) *.TMP
	$(MAKE) -C $(SUBDIR_TOOLS) clean

