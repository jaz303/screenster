CC			= gcc
CFLAGS		= -Isrc -I/usr/local/include -g
LDFLAGS		= -lallegro

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

OBJECTS		=	src/main.o	\
				src/rbuf.o

all: $(OBJECTS)
	$(CC) -o test $(OBJECTS) $(LDFLAGS)

clean:
	find . -name '*.o' -delete