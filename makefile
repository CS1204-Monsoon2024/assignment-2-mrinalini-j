default: main.cpp
	g++ -Wall main.cpp -o main.out  # write command to test your files (you do not need it for your submissions)
run: main.out 
	./main.out 
clean:
	rm *.out


