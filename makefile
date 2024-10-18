default: HashTable.cpp
	g++ -Wall HashTable.cpp -o HashTable.out  # write command to test your files (you do not need it for your submissions)
run: HashTable.out 
	./HashTable.out 
clean:
	rm *.out


