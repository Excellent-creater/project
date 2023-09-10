CC=gcc
STD=-std=gnu99
FLAG=-Wall -Werror
OBJ=02tcpS.o pthreadpool.o queue_array.o
BIN=POOL

all:$(OBJ)
	$(CC) $(OBJ) -o $(BIN) -lpthread && ./$(BIN)

%.o:%.c
	$(CC) -c $(STD) $(FLAG) $< -o $@

clean:
	rm -rf $(OBJ) $(BIN)
	rm -rf *~ *.h.gch
