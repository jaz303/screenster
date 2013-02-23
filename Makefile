CC			= gcc
CFLAGS		= -Isrc -I/usr/local/include -g --std=c99
LDFLAGS		= -L/usr/local/lib -lallegro -lallegro_main -lallegro_primitives -lallegro_image

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

OBJECTS		:=	src/graphics.o \
				src/handlers.o \
				src/main.o	\
				src/mpool.o \
				src/obj.o \
				src/builtin_types.o \
				src/state.o \
				src/msg_pool.o \
				src/buf.o

HANDLERS	:=	$(wildcard src/commands/*/*.c)

default: all

src/handlers.c: $(HANDLERS)
	rake generate_handlers
	touch src/main.c

all: src/handlers.c $(OBJECTS)
	$(CC) -o test $(OBJECTS) $(LDFLAGS)

clean:
	find . -name '*.o' -delete