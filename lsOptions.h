#ifndef _LSOPTIONS_H_
#define _LSOPTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <limits.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

// Print the inode number of the file
void optionI(struct stat* file);

// Print all the information about the file (permissions, number of links, 
// owner, group, size, last modification time, name)
void optionL(struct stat* file);

// Recursively print the contents of the directory
void optionR(int option, char* path);

// --------------------- HELPER FUNCTIONS --------------------- //
// Function to get the user name from the user id
void getAndPrintUserName(uid_t uid);

// Function to get the group name from the group id
void getAndPrintGroup(gid_t grpNum);

// Helper function to print the file name
void print_filename(struct dirent* directory);

// Helper function to print the file permissions
void print_permissions(struct stat* statbuffer);

// Helper function to clear the buffers and copy the path to new_Path
void clear_buffer(char symbolic[PATH_MAX], char new_Path[PATH_MAX], struct dirent* dir_entry, char* path);

#endif