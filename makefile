malloc537.o: malloc537.c malloc537.h rbtree.o
	gcc -g -Wall -pedantic -o malloc537.o malloc537.c rbtree.o
rbtree.o: rbtree.c rbtree.h
	gcc -g -Wall -pedantic rbtree.c
