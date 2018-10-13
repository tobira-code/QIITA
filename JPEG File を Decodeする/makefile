CFLAGS=-I. -Wall -g -mno-ms-bitfields
INCS=
OBJS=test.o
LIBS=
TARGET=test

all: $(TARGET)

%.o: %.c $(INCS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(TARGET) *.o
