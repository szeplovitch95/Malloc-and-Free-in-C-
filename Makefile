all:
	gcc mymalloc.c memgrind.c -o memgrind
mymalloc.o:
	gcc -c mymalloc.c
memgrid.o:
	gcc -c memgrid.c
clean:
	rm memgrind memgrind.o
