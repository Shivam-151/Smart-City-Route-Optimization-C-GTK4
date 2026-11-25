CC ?= gcc
CFLAGS += -Wall -Wextra -std=c11 $(shell pkg-config --cflags gtk4)
LDLIBS += $(shell pkg-config --libs gtk4) -lm
TARGET = smart-city-routes
SRC = src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET) smart-city-routes.exe

.PHONY: all clean
