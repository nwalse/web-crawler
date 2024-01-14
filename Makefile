CC = gcc
CFLAGS = -g
LIBS = -lcurl

TARGET = crawl
SRCS = crawler.c struct.c url.c curl.c pagedir.c
OBJS = $(SRCS:.c=.o)

.DEFAULT_GOAL := $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TARGET)

