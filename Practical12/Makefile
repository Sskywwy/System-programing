
CC = gcc

CFLAGS = -Wall -Wextra -MMD -MP


SRCS = app1.c utils.c


OBJS = $(SRCS:.c=.o)


DEPS = $(OBJS:.o=.d)


TARGET = app

build: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


-include $(DEPS)


run: build
	./$(TARGET)


clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)


.PHONY: build run clean