
#target $@
#dependencies $^

src = $(wildcard *.c)
obj = $(src:.c=.o)

shell: $(obj)
	gcc -o shell $^

%.o: %.c 
	gcc -c $^
	
	
clean: 
	rm *.o