division: main.o division.o
	g++ $(CFLAGS) -o division main.o division.o
    
main.o: src/main.c
	gcc $(CFLAGS) -c src/main.c
    
division.o: src/division.cpp include/division.hpp
	g++ $(CFLAGS) -c -I include/ src/division.cpp
    
clean: 
	rm -f *.o division
