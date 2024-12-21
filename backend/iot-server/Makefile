# Makefile

# Variables
BINARY_NAME = iot-server 
SOURCE_FILE = main.go garbage_bin.go util.go mqtt-sub.go influxdb.go

# Build the binary
build:
	go build -o $(BINARY_NAME) $(SOURCE_FILE)

# Run the binary
run: build
	./$(BINARY_NAME)

# Clean up the binary
clean:
	rm -f $(BINARY_NAME)

# Default target
all: build run
