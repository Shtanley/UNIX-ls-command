all:
	gcc -Wall -Werror lsOptions.c UnixLs.c -o UnixLs

test-single:
	./UnixLs; ./UnixLs -R; ./UnixLs -i; ./UnixLs -l

test-multiple:
	./UnixLs -i -l; ./UnixLs -i -R; ./UnixLs -l -R; ./UnixLs -i -R -l;

clean:
	rm -f UnixLs