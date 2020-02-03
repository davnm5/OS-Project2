all: prs test_list

prs: src/main.c
	gcc -static -c src/lru.c -o obj/lru.o
	ar -rcs lib/liblru.a obj/lru.o
	gcc -static -c src/fifo.c -o obj/fifo.o
	ar -rcs lib/libfifo.a obj/fifo.o
	gcc -static -c src/optimal.c -o obj/optimal.o
	ar -rcs lib/liboptimal.a obj/optimal.o
	gcc src/main.c -o bin/prs -lm -Llib -llru -lfifo -loptimal

test_list:
	gcc -static -c src/linkedlist.c -o obj/list.o
	ar -rcs lib/liblist.a obj/list.o
	gcc src/test_list.c -o bin/test_list -Llib -llist
clean:
	rm -rf obj/* bin/* lib/*