CC = gcc
CFLAGS = -Wall -O2 `pkg-config --cflags x11 xft`
LIBS = `pkg-config --libs x11 xft`

TARGET = enterprise-app
SRC = src/main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)
