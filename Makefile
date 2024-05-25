# CFLAGS=-Wall -Wextra -Isrc/ -lpaho-mqtt3c -lpthread
CFLAGS=-Isrc/ -lpaho-mqtt3c -lpthread
CC = gcc

TARGET_OUT=target
TARGET_FILES=./cmd/main.c

CLIENT_OUT=client
CLIENT_FILES=./cmd/client.c
UI_SRCS=$(wildcard ./ui/*.c)
CLIENT_CFLAGS=-Iui/ -lncurses 

SRC_FILES = $(wildcard ./src/**/*.c ./src/*.c)
SRC_OBJS= $(SRC_FILES:.c=.o)

TARGET_CFLAGS=-Imysensors/
SENSORS_SRC=$(wildcard ./mysensors/*.c)

all: target client

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Targets 
target: ${TARGET_FILES} $(SRC_OBJS) $(SENSORS_SRC)
	$(CC) ${CFLAGS} $(TARGET_CFLAGS) -o${TARGET_OUT} $(SRC_OBJS) ${TARGET_FILES} $(SENSORS_SRC)

client: $(CLIENT_FILES) $(SRC_OBJS) $(UI_SRCS)
	$(CC) ${CFLAGS} $(CLIENT_CFLAGS) $(SRC_OBJS) ${CLIENT_FILES} ${UI_SRCS} -o${CLIENT_OUT}

debug: CFLAGS += -DNDEBUG -g
debug: client 

# Run commands
run_temperature: target
	./${TARGET_OUT} temperature

run_client: client
	export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib
	./${CLIENT_OUT}

clean:
	rm -f $(TARGET_OUT)
	rm -f $(CLIENT_OUT)
	rm -f $(SRC_OBJS)
