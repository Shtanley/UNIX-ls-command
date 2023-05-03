#include "lsOptions.h"

void optionI(struct stat* file) {
	if(file != NULL) {
		printf("%ld    ", (long)file->st_ino);  // print inode number
	}
	return;
}

void optionL(struct stat* file) {
	// print the file's last modified time in the format: MMM DD YYYY HH:mm
	char time_buff[26];    // buffer to hold the time string
	memset(time_buff, '\0', 26*sizeof(char));
	struct tm* date_time = malloc(sizeof(struct tm));
    // Source: Stack Overflow
	localtime_r(&file->st_mtime, date_time);
	strftime(time_buff, 26, "\t%b %d %Y %H:%M", date_time);

	if(file != NULL) {  // print file permissions
		print_permissions(file);
		printf("%ld ", (long)file->st_nlink);// print number of hard links

		// print owner and group name
		getAndPrintUserName(file->st_uid);
		getAndPrintUserName(file->st_gid);

		printf("%ld", file->st_size); // print file size
		printf("%s ", time_buff);    // print last modified time
	}
	
	if(date_time != NULL) { // free the memory allocated for date_time
		free(date_time);
		date_time = NULL;
	}
	return;
}

void optionR(int option, char* path) {
	DIR* dir ;
	struct dirent* dir_entry;
	struct stat f_info;
	char* new_Path = malloc(sizeof(char)*PATH_MAX);
	char symbolic[PATH_MAX];
	char symlink[PATH_MAX];

	memset(&f_info, 0, sizeof(stat));
	dir = opendir(path);

	// check if the directory is valid
	if(dir == NULL)
		return;

	while((dir_entry = readdir(dir)) != NULL ) {	// read the directory
		// clear the buffers and copy the path to new_Path
		clear_buffer(symbolic, new_Path, dir_entry, path);
		// check if the directory entry is not "." or ".."
		if(strcmp(dir_entry->d_name,".") != 0 && strcmp(dir_entry->d_name,"..")) {
			lstat(new_Path,&f_info);

			if(option == 6 || option == 9)	// print inode number
				optionI(&f_info);

			if(option == 7 || option == 9)	// print all the information
				optionL(&f_info);

			print_filename(dir_entry);

			if(S_ISLNK(f_info.st_mode)) {	// if the file is a symbolic link
				if(option == 7 || option == 9){	// print the symbolic link
					ssize_t nbytes = readlink(new_Path, symbolic, PATH_MAX);
					if(nbytes != -1)
						printf(" -> %s",symbolic);
					printf("/");
				}
				else if(option == 6)	// print @
					printf("@ ");
				else	// print a space
					printf(" ");
			} 
			else	// print a space
				printf(" ");

			if(option != 4 && option != 6)	// print a new line
				printf("\n");

			if(option == 4 || option == 6)	// print a space
				printf(" ");
		}
	}
	printf("\n");
	memset(&f_info, 0, sizeof(stat));	// clear the buffer
	
	rewinddir(dir);	// reset the directory stream to the beginning
	while((dir_entry = readdir(dir)) != NULL) {	// read the directory
		// clear the buffers and copy the path to new_Path
		clear_buffer(symbolic, new_Path, dir_entry, path);
		memset(symlink, 0, sizeof(char)*PATH_MAX);
		
		lstat(new_Path, &f_info);		
		
		if(S_ISLNK(f_info.st_mode)) {	// if the file is a symbolic link
			continue;	// skip the file
		}
		// check if the directory entry is not "." or ".."
		if(strcmp(dir_entry->d_name,".") != 0 && strcmp(dir_entry->d_name,"..")) {
			if(S_ISDIR(f_info.st_mode)) {	// if the file is a directory
				printf("%s:\n", new_Path);	// print the path
				optionR(option, new_Path);	// call optionR recursively
			}
		}
	}
	closedir(dir);	// close the directory stream
	if(new_Path != NULL) {	// free the memory allocated for new_Path
		free(new_Path);
		new_Path = NULL;
	}
	return;
}

// --------------------- HELPER FUNCTIONS --------------------- //

void getAndPrintUserName(uid_t uid) {
	
	struct passwd *pw = NULL;
	pw = getpwuid(uid);

	if (pw) {
		printf("%s ",pw->pw_name);
	}
}

void getAndPrintGroup(gid_t grpNum) {
	
	struct group *grp;
	
	grp = getgrgid(grpNum);  

	if (grp) {
		printf("%s ",grp->gr_name);
	}
}

void print_filename(struct dirent* directory) {
	if(directory != NULL)
		printf("%s", directory->d_name);	
	return;
}

// Source: Stack Overflow [1]
void print_permissions(struct stat* file) {
    if (S_ISDIR(file->st_mode))
        printf("d"); // directory File
    else if (S_ISLNK(file->st_mode))
        printf("l"); // symbolic link
	// else if (S_ISREG(file->st_mode)) // regular file
	
    printf( (S_ISDIR(file->st_mode)) ? "d" : "-");
    printf( (file->st_mode & S_IRUSR) ? "r" : "-");
    printf( (file->st_mode & S_IWUSR) ? "w" : "-");
    printf( (file->st_mode & S_IXUSR) ? "x" : "-");
    printf( (file->st_mode & S_IRGRP) ? "r" : "-");
    printf( (file->st_mode & S_IWGRP) ? "w" : "-");
    printf( (file->st_mode & S_IXGRP) ? "x" : "-");
    printf( (file->st_mode & S_IROTH) ? "r" : "-");
    printf( (file->st_mode & S_IWOTH) ? "w" : "-");
    printf( (file->st_mode & S_IXOTH) ? "x" : "-");
    printf("  ");
}

void clear_buffer(char symbolic[PATH_MAX], char new_Path[PATH_MAX], struct dirent* dir_entry, char* path) {
	memset(symbolic, 0, sizeof(char)*PATH_MAX);
	memset(new_Path, 0, sizeof(char)*PATH_MAX);
	strcpy(new_Path, path);
	strcat(new_Path,"/");
	strcat(new_Path, dir_entry->d_name);
}

