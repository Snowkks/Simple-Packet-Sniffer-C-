CC = gcc

TARGET = sniffer

SRC = packet_sniffer.c

all:
	$(CC) $(SRC) -o $(TARGET)
	@echo "Build successful! Run with: sudo ./$(TARGET)"

clean:
	rm -f $(TARGET)
