                                    	UNIX ls command in C
____________________________________________________________________________________________________

Deliverables:
    1. UnixLs.C
    2. UnixLs.h
    3. lsOptions.h
    4. lsOptions.C
    5. Makefile
    6. README

Makefile Instructions:
    1. make all - compiles the program
    2. make clean - removes all the object files and the executable
    3. make test-single - tests ls with each option, -i, -l, -R including no options
    4. make test-multiple - tests ls command contating multiple options (ls -i -l, ls -i -R -L, etc.)

Command line Instructions:
    - Syntax: ./UnixLs -[i/l/R] [file/directory]
    - Example: ./UnixLs -i -l -R /home/username
    1. -i - prints the inode number of each file
    2. -l - prints the long listing format
    3. -R - recursively lists subdirectories encountered

Bugs:
    1. The program does not print with the exact format as the actual ls command.
    2. There are minor bugs with printing symbolic links and the file size.

References:
	1. https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c