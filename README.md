# UNIX ls command in C

Deliverables:\
&ensp; 1. UnixLs.C\
&ensp; 2. UnixLs.h\
&ensp; 3. lsOptions.h\
&ensp; 4. lsOptions.C\
&ensp; 5. Makefile\
&ensp; 6. README

Makefile Instructions:\
&ensp; 1. make all - compiles the program\
&ensp; 2. make clean - removes all the object files and the executable\
&ensp; 3. make test-single - tests ls with each option, -i, -l, -R including no options\
&ensp; 4. make test-multiple - tests ls command contating multiple options (ls -i -l, ls -i -R -L, etc.)

Command line Instructions:\
&ensp; - Syntax: ./UnixLs -[i/l/R] [file/directory]\
&ensp; - Example: ./UnixLs -i -l -R /home/username\
&ensp; 1. -i - prints the inode number of each file\
&ensp; 2. -l - prints the long listing format\
&ensp; 3. -R - recursively lists subdirectories encountered

Bugs:\
&ensp; 1. The program does not print with the exact format as the actual ls command.\
&ensp; 2. There are minor bugs with printing symbolic links and the file size.

References:\
&ensp; 1. https://stackoverflow.com/questions/10323060/printing-file-permissions-like-ls-l-using-stat2-in-c
