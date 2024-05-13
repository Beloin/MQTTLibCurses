CFLAGS=-Wall -Wextra -Isrc/ -lpaho-mqtt3c
CC = gcc

TARGET_OUT=target
TARGET_FILES=./cmd/main.c

CLIENT_OUT=client
CLIENT_FILES=./cmd/client.c
UI_SRCS=$(wildcard ./ui/*.c)
CLIENT_CFLAGS=-Iui/ -lncurses

SRC_FILES = $(wildcard ./src/**/*.c) $(wildcard ./src/*.c)
SRC_OBJS= $(SRC_FILES:.c=.o)

all: target client

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Targets 
target: ${TARGET_FILES} $(SRC_OBJS)
	$(CC) ${CFLAGS} -o${TARGET_OUT} $(SRC_OBJS) ${TARGET_FILES}

client: $(CLIENT_FILES) $(SRC_OBJS) 
	$(CC) ${CFLAGS} $(CLIENT_CFLAGS) -o${CLIENT_OUT} $(SRC_OBJS) ${CLIENT_FILES} ${UI_SRCS}

# Run commands
run_temperature: target
	./${TARGET_OUT} temperature

run_client: client
	./${CLIENT_OUT}

clean:
	rm -f $(TARGET_OUT)
	rm -f $(CLIENT_OUT)
	rm -f $(SRC_OBJS)
