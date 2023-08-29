CC=gcc
STD=-std=gnu99
FLAG=-Wall -Werror
OBJ=chat_client.o my_network.o
BIN=client
LIB=-pthread

all:$(OBJ)
	$(CC) $(OBJ) $(LIB) -o $(BIN) && ./$(BIN) 127.0.0.1 6666

%.o:%.c
	$(CC) $(STD) $(FLAG) -c $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)
	rm -rf *.h.gch *~
