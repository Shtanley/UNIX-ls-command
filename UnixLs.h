#ifndef UNIXLS_H
#define UNIXLS_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <locale.h>
#include <langinfo.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>

// Get the user's input and store it in an array
char** get_input(int size, char* arr[]);

// Check the user's input and store the options in an array
// void check_info(int len, char *argv[]);

#endif