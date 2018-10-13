CFLAGS=-I. -Wall -Werror -O2
INCS=
OBJS=test.o test_s.o
LIBS=
TARGET=test

all: $(TARGET)

%.o: %.c $(INCS)
	$(CC) $(CFLAGS) -c -o $@ $<
	objdump -d $@ > $@.disas

%.o: %.s $(INCS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
	objdump -d $@.exe > $@.exe.disas

clean:
	rm -rf $(TARGET) *.o *.disas
