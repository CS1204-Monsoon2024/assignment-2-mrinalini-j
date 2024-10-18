default: main

main: main.o HashTable.o
    g++ -o main main.o HashTable.o

main.o: main.cpp
    g++ -c main.cpp

HashTable.o: HashTable.cpp HashTable.h
    g++ -c HashTable.cpp

clean:
    rm -f *.o main

