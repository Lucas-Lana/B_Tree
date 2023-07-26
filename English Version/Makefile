DEBUG := -Wall -Wextra -pedantic -ggdb


all: B_Tree.o main.o
	gcc -o $@ $^

main.o: src/main.c
	gcc $(DEBUG) -c $<

B_Tree.o: src/B_Tree.c src/B_Tree.h
	gcc $(DEBUG) -c $<

clean:
	$(RM) ./*.o ./all