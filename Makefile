build:
	gcc -o tema3 test.c

run:
	./tema3

tests:
	gcc -o test test.c -Wall && ./test

diff:
	diff output.out refout && diff error.out referr

valgrind:
	gcc -o tema3 test.c && valgrind --tool=memcheck --leak-check=yes ./tema3
