CC = g++
CFLAGS  = -pthread

SERVER = main




all:  $(SERVER)

$(SERVER): $(SERVER).cpp
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER).cpp
clean:
	$(RM) $(TARGET)

