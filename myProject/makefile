run: run.o macro.o code.o bit.o tools.o linked_list.o errors.o
	gcc -ansi -pedantic -Wall run.o macro.o code.o bit.o tools.o linked_list.o errors.o -o run
run.o: run.c linked_list.h
	gcc -c -ansi -Wall -pedantic run.c -o run.o
macro.o:macro.c errors.h
	gcc -c -ansi -Wall -pedantic macro.c -o macro.o
code.o:code.c errors.h
	gcc -c -ansi -Wall -pedantic code.c -o code.o
bit.o:bit.c errors.h
	gcc -c -ansi -Wall -pedantic bit.c -o bit.o
tools.o:tools.c errors.h
	gcc -c -ansi -Wall -pedantic tools.c -o tools.o
linked_list.o:linked_list.c errors.h
	gcc -c -ansi -Wall -pedantic linked_list.c -o linked_list.o
errors.o:errors.c errors.h
	gcc -c -ansi -Wall -pedantic errors.c -o errors.o
