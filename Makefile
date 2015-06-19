all:
	gcc autojoin.c -pedantic -Wall -Wextra \
		`pkg-config --cflags mcabber` \
		`pkg-config --libs x11` -lxdo \
		-std=gnu99 \
		-shared -DMODULES_ENABLE  -fPIC\
		-o libautojoin.so

install:
	install -D libautojoin.so /usr/lib/mcabber/libautojoin.so
