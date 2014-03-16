#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <limits.h>

int getLine(const char* filename) {
	FILE* fp;
	int count = 0;
	char c;
	fp = fopen(filename, "r");
	if(fp == NULL)
		return -1;
	while((c = (char)fgetc(fp)) != EOF) {
		if(c == '\n')
			count++;
	}
	fclose(fp);
	return count;
}

int retrieve_directory(char* path, char* target, int recursive) {
	int total = 0;
	struct dirent* dent;
	DIR* dir = opendir(path);
	if(dir == NULL) {
		printf("%s open error\n", path);
		return 0;
	}
	while((dent = readdir(dir)) != NULL) {
		char full_name[PATH_MAX + 1];
		strcpy(full_name, path);
		strcat(full_name, "/");
		strcat(full_name, dent->d_name);

		if(dent->d_name[0] == '.' && 
				(dent->d_name[1] == '\0' || 
				 (dent->d_name[1] == '.' && dent->d_name[2] == '\0')))
			continue;

		if(dent->d_type == DT_DIR) {
			if(recursive != 0) {
				// Recursive retrieve
				total += retrieve_directory(full_name, target, recursive);
			}
		} else {
			if(recursive == 1 || strcmp(target, dent->d_name) == 0) {
				int lines;
				char fullpath[PATH_MAX + 1];
				realpath(dent->d_name, fullpath);

				lines = getLine(full_name);
				if(lines <= 0) continue;
				printf("%s:\t%d Lines\n", full_name, lines);
				total += lines;
			}
		}
	}
	closedir(dir);
	return total;
}

int main(int argc, char** argv) {
	int total = 0;
	{
		if(argc < 2) {
			printf("Invalid arguments\n");
			printf("Usage:\n\tcodeline [filename] [filename] ...\n");
			printf("\tcodeline -R\n");
			return 0;
		}
	}

	{
		const char RECURSIVE_FLAG[] = "-R";
		if(argc == 2 && (strncmp(argv[1], RECURSIVE_FLAG, strlen(RECURSIVE_FLAG)) == 0)) {
			total += retrieve_directory(".", NULL, 1);
		} else {
			int i;
			for(i = 1; i < argc; i++) {
				total += retrieve_directory(".", argv[i], 0);
			}
		}
	}
	printf("------------------------\n");
	printf("Total:\t%d Lines\n", total);

	return 0;
}
