CC = g++
CFLAGS  = -pthread -std=c++20

SERVER = main




all:  $(SERVER)

$(SERVER): $(SERVER).cpp
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).cpp
clean:
	$(RM) $(TARGET)

