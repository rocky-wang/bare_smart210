#Design by Rocky for build add checksum for bare smart210 binary

TARGET	:= mk210header

COBJS	:= mk210_header.o

CROSS_COMPILE	:= 
CC				:= $(CROSS_COMPILE)gcc

CFLAGS	+= -Wall -O2

LDFLAGS	+= 

all:$(TARGET)
$(TARGET):$(COBJS)
	$(CC) $(LDFLAGS) -o $@ $^

%o:%c
	$(CC) -c $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
