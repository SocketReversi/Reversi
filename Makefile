CC = gcc -Wall
CFLAGS = -I
LIBS = -lm
SERVER = server
CLIENT = client
GTK = `pkg-config --libs --cflags gtk+-3.0`
EXP = -export-dynamic
# Header file
DEPS = $(wildcard lib/*.h)

# Source file
source = $(wildcard src/*.c)
source_services = $(wildcard src/services/*.c)
source_ultilities = $(wildcard src/ultilities/*.c)
source_validations = $(wildcard src/validations/*.c)

# Object file
main = $(patsubst %.c,%.o,$(source))
services = $(patsubst %.c,%.o,$(source_services))
ultilities = $(patsubst %.c,%.o,$(source_ultilities))
validations = $(patsubst %.c,%.o,$(source_validations))

# Lib
libs = $(services) $(ultilities) $(validations)

# Run command
all: $(SERVER) $(CLIENT)

$(SERVER): src/$(SERVER).o $(libs)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(GTK)

$(CLIENT): src/$(CLIENT).o $(libs) 
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) $(GTK) $(EXP)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(GTK)

clean: $(libs) $(main) $(SERVER) $(CLIENT)
	rm -rf $(libs) $(main) $(SERVER) $(CLIENT)
