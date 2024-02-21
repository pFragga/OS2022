CC = cc
CFLAGS = -Wall -Wextra -pthread
TARGET = test-res

all: $(TARGET)

$(TARGET): 3200234-3200056-res.c 3200234-3200056-res.h
	$(CC) $(CFLAGS) $< -o $@ 

clean:
	rm -f test-res

.PHONY: all clean
