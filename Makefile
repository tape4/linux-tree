mytree.out : mytree.o
	gcc -o mytree.out mytree.o

mytree.o : mytree.c
	gcc -c -o mytree.o mytree.c

clean:
	rm mytree.o mytree.out
