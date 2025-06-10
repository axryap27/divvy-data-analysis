build:
	rm -f ./a.out
	gcc -std=c11 -g -Wall -pedantic -Werror main.c dist.c -lm -Wno-unused-variable -Wno-unused-function 

run:
	./a.out

valgrind:
	rm -f ./a.out
	gcc -std=c11 -g -Wall -pedantic -Werror main.c dist.c -lm -Wno-unused-variable -Wno-unused-function 
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./a.out

submit:
	/home/cs211/s2025/tools/projectxc submit *.c *.h

commit:
	git add .
	git commit -m "$(msg)"

push:
	git push origin main

