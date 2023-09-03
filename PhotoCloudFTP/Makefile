CC=gcc
STD=-std=gnu99
FLAG=-Wall -Werror
OBJ=ftp_client.o main.o tools.o
BIN=FTP

all:$(OBJ)
	$(CC) $(OBJ) -o $(BIN) && ./$(BIN) 127.0.0.1

%.o:%.c
	$(CC) -c $(STD) $(FLAG) $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)
	rm -rf *~ *.h.gch

