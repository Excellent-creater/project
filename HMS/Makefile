CC=g++
FLAG=-Wall -Werror
OBJ= hotel.o main.o tools.o
BIN=HMS

all:$(OBJ)
	$(CC) $(OBJ) -o $(BIN) && ./$(BIN) hotel_profile.txt

main.o:main.cpp hotel.h tools.h
	$(CC) $(FLAG) -c main.cpp -o main.o
hotel.o:hotel.cpp hotel.h room.h guest.h tools.h
	$(CC) $(FLAG) -c hotel.cpp -o hotel.o
tools.o:tools.cpp tools.h
	$(CC) $(FLAG) -c tools.cpp -o tools.o

clean:
	rm -rf $(OBJ) $(BIN)
	rm -rf *~ *.h.gch
