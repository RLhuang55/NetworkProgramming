CC = gcc
CFLAGS = -Wno-implicit-function-declaration -g -MD
LFLAGS = -lreadline
INC = -Iinclude

SRCS = $(wildcard src/*.c)

OBJS = $(patsubst src/%.c, obj/%.o, ${SRCS})

.PHONY: depend clean all
DEPS := $(OBJS:.o=.d)

all: shell

-include $(DEPS)

shell:	$(OBJS)
	@$(CC) ${CFLAGS} ${SRCS} ${LFLAGS} -o shell

obj/%.o:src/%.c
	@$(CC) $(CFLAGS) $(INC) -c -o $@ $<
clean:
	@rm obj/*.o obj/*.d