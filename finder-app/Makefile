GCC=$(CROSS_COMPILE)gcc

writer: writer.o
	$(GCC) writer.o -o writer

writer.o: writer.c
	$(GCC) -c writer.c -o writer.o

clean:
	rm -f *.o *.elf writer
