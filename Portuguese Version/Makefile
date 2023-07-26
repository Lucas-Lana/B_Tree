DEBUG := -Wall -Wextra -pedantic -ggdb


all: Arvore_B.o main.o
	gcc -o $@ $^

main.o: src/main.c
	gcc $(DEBUG) -c $<

Arvore_B.o: src/Arvore_B.c src/Arvore_B.h
	gcc $(DEBUG) -c $<

clean:
	$(RM) ./*.o ./all