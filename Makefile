todo: src/main.c
		gcc -Iinclude/ src/main.c src/todo_list.c -o todo -g -O0

test: src/test.c
		gcc -Iinclude/ src/test.c src/todo_list.c -o test -g -O0

clean:
		rm -f todo test
