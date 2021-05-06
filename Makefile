CFLAGS += -Isrc -fPIC
SRC += $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

ifeq ($(shell uname), Darwin)
AR = /usr/bin/libtool
AR_OPT = -static $^ -o $@
else
AR = ar
AR_OPT = rcs $@ $^
endif

all: static dynamic demo

.PHONY: static dynamic
static: libcmdapp.a
dynamic: libcmdapp.so

libcmdapp.a: ${OBJ}
	${AR} ${AR_OPT}

libcmdapp.so: ${OBJ}
	${CC} -shared $< -o $@

demo: libcmdapp.a
	${CC} ${CFLAGS} main.c -L. -lcmdapp -o main

.c.o:
	${CC} ${CFLAGS} $< -c -o ${<:.c=.o}

clean:
	rm -rf libcmdapp.a libcmdapp.so ${OBJ}
