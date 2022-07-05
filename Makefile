all: test

test:
	gcc -c -fprofile-arcs -ftest-coverage -g circbuf.c
	g++ -c test_circbuf.cpp --std=c++11
	g++ -o test_circbuf.exe circbuf.o test_circbuf.o -lcppunit --std=c++11 -fprofile-arcs
	./test_circbuf.exe
	gcov circbuf

clean:
	rm circbuf.o test_circbuf.o test_circbuf.exe *.gcda *.gcno *.gcov
