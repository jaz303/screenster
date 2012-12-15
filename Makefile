CC			= gcc
CFLAGS		= -Isrc -I/usr/local/include -g -std=c99
LDFLAGS		= -lallegro -lallegro_primitives

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

OBJECTS		:=	src/main.o	\
				src/rbuf.o

HANDLERS	:=	$(wildcard src/commands/*/*.c)

default: all

src/handlers.c: $(HANDLERS)
	rake generate_handlers
	touch src/main.c

all: src/handlers.c $(OBJECTS)
	$(CC) -o test $(OBJECTS) $(LDFLAGS)

clean:
	find . -name '*.o' -delete