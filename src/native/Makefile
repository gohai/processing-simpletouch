TARGET := libprocessing-simpletouch.so
OBJS := impl.o
CC := gcc

# add -m32 to compile for x86 on x64
CFLAGS := -std=c99 -fPIC -g
CFLAGS += -I$(shell dirname $(shell realpath $(shell which javac)))/../include
CFLAGS += -I$(shell dirname $(shell realpath $(shell which javac)))/../include/linux
CFLAGS += $(shell pkg-config libevdev --cflags)
LDFLAGS := -shared
LDFLAGS += $(shell pkg-config libevdev --libs)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

iface.h:
	javah -classpath .. -o iface.h processing.simpletouch.SimpleTouch

clean:
	rm -f $(TARGET) $(OBJS)
