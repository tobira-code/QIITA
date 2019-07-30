CFLAGS=-I. -Wall -Werror -O1 -g
INCS=
OBJS=test.o test_s.o
LIBS=
TARGET=test

all: $(TARGET)

%.o: %.c $(INCS)
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.s $(INCS)
	$(CC) $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(TARGET) *.o
