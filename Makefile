mytree : mytree.o
	gcc -o mytree mytree.o

mytree.o : mytree.c
	gcc -c -o mytree.o mytree.c

clean:
	rm mytree.o mytree.out
