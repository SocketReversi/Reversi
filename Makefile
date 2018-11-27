CC = gcc
CFLAGS = -I
LIBS = -lm
SERVER = server
CLIENT = client

# Header file
DEPS = $(wildcard lib/*.h)

# Source file
source = $(wildcard src/*.c)
source_services = $(wildcard src/services/*.c)
source_ultilities = $(wildcard src/ultilities/*.c)
source_validations = $(wildcard src/validations/*.c)
output = $(wildcard output/*)
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
	$(CC) -o output/$@ $^ $(CFLAGS) $(LIBS)

$(CLIENT): src/$(CLIENT).o $(libs)
	$(CC) -o output/$@ $^ $(CFLAGS) $(LIBS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< 

clean: $(libs) $(main) $(output)
	rm -rf $(libs) $(main) $(output)