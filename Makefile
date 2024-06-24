.PHONY: all clean

FLAGS=$(shell pkg-config --libs --cflags gio-2.0 gio-unix-2.0 glib-2.0)

all: server client

gen:
	gdbus-codegen --generate-c-code mydbus --c-namespace namespaceDBUS --interface-prefix com.DBUS_gen. com.DBUS_gen.xml

%.o: %.c
	gcc -o $@ $^ -c $(FLAGS)

server: server.o mydbus.o
	gcc -o $@ $^ $(FLAGS)

client: client.o mydbus.o
	gcc -o $@ $^ $(FLAGS)

clean:
	rm -f *.o server client mydbus.c mydbus.h
