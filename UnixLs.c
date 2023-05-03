#include "UnixLs.h"
#include "lsOptions.h"

static char** in_buff = NULL;		// input buffer to hold directory from user input
static int* file_index = NULL;		// array of index of folder from user-input
static int in_options[3];			// input options array: 0 = i; 1 = l; 2 = R 
static int option = 0; 				// user input option
static int sym_sz = 0;				// size of sym_link array
static char sym_link[PATH_MAX];		// array containing actual path of symbolic link

int main(int argc, char* argv[]) {
	DIR *dir;  					// directory pointer
	struct dirent* dir_entry;	// directory entry
	struct stat f_info;   		// file information
	char** in_dir;   			// array of input directory
	char new_Path[PATH_MAX];    // new path for each file

	// Default case, no options
    // Print all the files in the current working directory
	if(argc <= 1) {
		dir = opendir(".");

		while(dir){
			if((dir_entry = readdir(dir)) != NULL) {
				if( strcmp(dir_entry->d_name,".") != 0 && strcmp(dir_entry->d_name,"..")){
					print_filename(dir_entry);
					printf("  ");
				}
			}
			else {
				printf("\n");
				closedir(dir);
				return 0;
			}
		}
		closedir(dir);
	} else {    // User input case
		in_dir = get_input(argc, argv);   // get user input

		if(sym_sz >= 2){    // if more than 1 directory
			for(int i = 0; i < sym_sz; i++) {
				memset(&f_info, 0, sizeof(struct stat));    // clear the memory
				lstat(in_dir[i], &f_info);    // read the file information

				if(option == 2 || option == 5 || option == 6 || option == 9)    // print i-option
					optionI(&f_info);

				if(option == 3 || option == 5 || option == 7 || option == 9)    // print l-option
					optionL(&f_info);

				printf("%s", argv[file_index[i]]);

				if(S_ISLNK(f_info.st_mode)) {
					if(option == 3 || option == 5 || option == 7 || option == 9) {   // print l-option
						ssize_t nbytes = readlink(in_dir[i],sym_link, PATH_MAX);
						if(nbytes != -1)
							printf(" -> %s",sym_link);
						printf("/");
					}

					else if(option == 2 || option == 4 || option == 6)  // print i-option
						printf("@ ");
					else
						printf(" ");

				} else
					printf(" ");

				if(option == 3 || option == 5 || option == 7|| option == 9)   // print l-option
					printf("\n");
				else
					printf(" ");
			}
			printf("\n");
		}

		//loop & read information of all valid directory
		for(int i = 0; i < sym_sz; i++) {
			memset(&f_info, 0, sizeof(struct stat));    // clear the memory
			dir = opendir(in_dir[i]);    // open the directory

			if(!dir)    // if the directory is not valid
				continue;

			printf("%s:\n", in_dir[i]);  // print the directory name

			if(option != 4 && option != 6 && option != 7 && option != 9) {  // if non recursive option
				while( (dir_entry = readdir(dir)) != NULL){
					memset(new_Path, 0, sizeof(char)*PATH_MAX); // clear the memory
					strcat(new_Path, in_dir[i]);    // add the directory name to the path
					strcat(new_Path,"/");   // add the "/" to the path
					strcat(new_Path, dir_entry->d_name);   // add the file name to the path

					if( strcmp(dir_entry->d_name,".") != 0 && strcmp(dir_entry->d_name,"..")) {
						lstat(new_Path, &f_info);

						if(option == 2 || option == 5)  // print i-option
							optionI(&f_info);

						if(option == 3 || option == 5)  // print l-option
							optionL(&f_info);
					
						print_filename(dir_entry);

						if(S_ISLNK(f_info.st_mode)){    // symbolic link
							if(option == 3 || option == 5) { // print l-option
								ssize_t nbytes = readlink(new_Path,sym_link, PATH_MAX);
								if(nbytes != -1)
									printf(" -> %s",sym_link);
								printf("/");
							}
							else if(option == 2)
								printf("@ ");
							else
								printf(" ");
						} else
							printf(" ");

						if(option == 3 || option == 5 || option == 7 || option ==9)   // print l-option
							printf("\n");
					}	
				}

                if(option != 4 && option != 6)  // print new line
                    printf("\n");		
			} else {    // recursive option
				optionR(option, in_dir[i]);
			}

			if(dir) 
                closedir(dir);  // close the directory
		}

	}

	if(in_buff != NULL) {	// free the memory
		for(int i = 0; i < sym_sz; i++) {   // free the input directory
			memset(in_buff[i], 0, sizeof(char)*PATH_MAX);
			free(in_buff[i]);
			in_buff[i] = NULL;
		}

		free(in_buff);  // free the input buffer
		in_buff = NULL;
	}

	if(file_index != NULL){ // free the file index
		free(file_index);
		file_index = NULL;
	}
	
	memset(new_Path, 0, sizeof(char)*PATH_MAX);
	return 0;
}

char** get_input(int size, char* list[]) {
	memset(in_options, 0, sizeof(int)*3);   // clear the memory
	int index_Path = 0;

	for(int i = 1; i < size; i++){  // loop through all the user arguments
		if(list[i][0] != '-')
			sym_sz++;
	}
    // allocate the memory
	in_buff = malloc(sizeof(char*)* sym_sz);
	file_index = malloc(sizeof(int)*sym_sz);
	memset(file_index, 0, sizeof(int)*sym_sz);

	for(int i = 1; i < size; i++) { // loop through all the user arguments
		if(list[i][0] == '-') { // if user provides the option
			for(int j = 0; j < strlen(list[i]); j++) {  // loop through the options
				if(list[i][j] == 'i')	// option i = index 0
					in_options[0] = 1;
				if(list[i][j] == 'l')	// option l = index 1
					in_options[1] = 1;
				if(list[i][j] == 'R')	// option R = index 2
					in_options[2] = 1;
			}
		} else {    // if user provides the link
			// allocate the memory for the link
			in_buff[index_Path] = malloc(sizeof(char)*PATH_MAX);
			memset(in_buff[index_Path], 0, sizeof(char)*PATH_MAX);
			file_index[index_Path] = i; 
            // check if the link is valid
			char* check = realpath(list[i], in_buff[index_Path]);
			if(!check) { // if link is not valid
				char err_Message[100]; 
				strcat(err_Message, "UnixLs: cannot access '");
				strcat(err_Message, list[i]);
				strcat(err_Message, "'");
				perror(err_Message);

				memset(err_Message, 0, sizeof(char)*PATH_MAX);
				exit(EXIT_FAILURE);
			}
		
			index_Path++;
		}
	}

	if(sym_sz == 0) {   // if user does not provide any link
        // allocate the memory for the link
		in_buff = malloc(sizeof(char*));
		in_buff[0] = malloc(sizeof(char)*1);
		strcat(in_buff[0],".");
		sym_sz++;
	}

	// calculate the option
    // 1 = i, 2 = l, 3 = il, 4 = R, 5 = Ri, 6 = Rl, 7 = Ril
	option = in_options[0]*2 + in_options[1]*3 + in_options[2]*4;
	return in_buff;
}

