cc = gcc
objs = temp/main.o temp/list.o
dirs = temp

main: $(objs)
	$(cc) -o a.out $(objs)

temp/main.o: main.c
	$(cc) -c -g -o temp/main.o main.c

temp/list.o: List/List.h List/list.c
	$(cc) -c -g -o temp/list.o List/list.c

.PHONY: clean

clean:
	rm -rf temp ./*.o a.out 

$(shell mkdir -p $(dirs))