my_cond:my_cond.c
	gcc -o $@ $^ -lpthread
.PHONY:clean
clean:
	rm -f my_cond
