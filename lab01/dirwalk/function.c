#include "function.h"

void walkdir(char* path, char*** entries, int* count, int* capacity) {

	DIR *dir = opendir(path);

	if (dir == NULL) {
        perror("Ошибка при открытии каталога");
        return;
    }

	(*entries)[*count] = malloc(strlen(path) + 1);
	strcpy((*entries)[*count], path);
	(*count)++;

	struct dirent *entry;
	struct stat* statbuf;

	while ((entry = readdir(dir)) != NULL) {

		if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) {
			continue;
		}

		if (entry->d_type == DT_UNKNOWN) {

			char fullpath[PATH_MAX] = "";
			snprintf(fullpath, PATH_MAX, "%s/%s", path, entry->d_name);

			if (lstat(fullpath, statbuf) != 0) {
				perror("Ошибка вызова stat");
				exit(EXIT_FAILURE);
			}

			(*entries)[*count] = fullpath;
			(*count)++;

			if (*count >= *capacity * 0.7) {
				memInc(entries, capacity);
			}

			if (S_ISDIR(statbuf->st_mode)) {
				walkdirL(fullpath, entries, count, capacity);
			}

		} else if (entry->d_type == DT_DIR) {

			char newPath[PATH_MAX] = "";
			snprintf(newPath, PATH_MAX, "%s/%s", path, entry->d_name);

			walkdir(newPath, entries, count, capacity);

		} else {

			(*entries)[*count] = malloc(strlen(path) + strlen(entry->d_name) + 2);
			
			if ((*entries)[*count] == NULL) {
				perror("Ошибка выделения памяти");
				exit(EXIT_FAILURE);
			} 

			snprintf((*entries)[*count], strlen(path) + strlen(entry->d_name) + 2, "%s/%s", path, entry->d_name);
			(*count)++;

			if (*count >= *capacity * 0.7) {
				memInc(entries, capacity);
			}
		}
	}

    if (closedir(dir) != 0) {

        perror("Ошибка при закрытии каталога");
        return;

    }
}

void walkdirL(char* path, char*** entries, int* count, int* capacity){

	DIR *dir = opendir(path);

	if (dir == NULL) {
        perror("Ошибка при открытии каталога");
        return;
    }

	struct dirent *entry;
	struct stat* statbuf;
	char* fullpath = NULL;

	while ((entry = readdir(dir)) != NULL) {

		if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) {
			continue;
		}

		fullpath = malloc(strlen(path) + strlen(entry->d_name) + 2);

		if (fullpath == NULL) {
			perror("Ошибка выделения памяти");
			exit(EXIT_FAILURE);
		}

		snprintf(fullpath, strlen(path) + strlen(entry->d_name) + 2, "%s/%s", path, entry->d_name);

		if (entry->d_type == DT_UNKNOWN) {

			if (lstat(fullpath, statbuf) != 0) {
				perror("Ошибка вызова stat");
				exit(EXIT_FAILURE);
			}

			if (S_ISLNK(statbuf->st_mode)) {

				(*entries)[*count] = fullpath;
				(*count)++;

				if (*count >= *capacity * 0.7) {
					memInc(entries, capacity);
				}

			} else if (S_ISDIR(statbuf->st_mode)) {

				walkdirL(fullpath, entries, count, capacity);

				free(fullpath);

			} else {
				free(fullpath);
			}

		} else if (entry->d_type == DT_LNK) {

			(*entries)[*count] = fullpath;
			(*count)++;

			if (*count >= *capacity * 0.7) {
				memInc(entries, capacity);
			}

		} else if (entry->d_type == DT_DIR) {
			walkdirL(fullpath, entries, count, capacity);
			free(fullpath);
		} else {
			free(fullpath);
		}

	}

    if (closedir(dir) != 0) {
        perror("Ошибка при закрытии каталога");
        return;
    }
}

void walkdirD(char* path, char*** entries, int* count, int* capacity){

	DIR *dir = opendir(path);

	if (dir == NULL) {
        perror("Ошибка при открытии каталога");
        return;
    }

	if (*count == 0) {

		(*entries)[*count] = malloc(strlen(path) + 1);
		strcpy((*entries)[*count], path);
		(*count)++;

	}

	struct dirent *entry;
	struct stat* statbuf;
	char* fullpath = NULL;

	while ((entry = readdir(dir)) != NULL) {

		if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) {
			continue;
		}

		fullpath = malloc(strlen(path) + strlen(entry->d_name) + 2);

		if (fullpath == NULL) {
			perror("Ошибка выделения памяти");
			exit(EXIT_FAILURE);
		}

		snprintf(fullpath, strlen(path) + strlen(entry->d_name) + 2, "%s/%s", path, entry->d_name);

		if (entry->d_type == DT_UNKNOWN) {

			if (lstat(fullpath, statbuf) != 0) {
				perror("Ошибка вызова stat");
				exit(EXIT_FAILURE);
			}

			if (S_ISDIR(statbuf->st_mode)) {

				(*entries)[*count] = fullpath;
				(*count)++;

				if (*count >= *capacity * 0.7) {
					memInc(entries, capacity);
				}

				walkdirD(fullpath, entries, count, capacity);
			}

		}else if (entry->d_type == DT_DIR) {

			(*entries)[*count] = fullpath;
			(*count)++;

			if (*count >= *capacity * 0.7) {
				memInc(entries, capacity);
			}

			walkdirD(fullpath, entries, count, capacity);

		} else {
			free(fullpath);
		}

	}

    if (closedir(dir) != 0) {
        perror("Ошибка при закрытии каталога");
        return;
    }
}

void walkdirF(char* path, char*** entries, int* count, int* capacity){

	DIR *dir = opendir(path);

	if (dir == NULL) {

        perror("Ошибка при открытии каталога");
        return;

    }

	struct stat* statbuf;
	struct dirent *entry;
	char* fullpath = NULL;

	while ((entry = readdir(dir)) != NULL) {

		if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0) {
			continue;
		}

		fullpath = malloc(strlen(path) + strlen(entry->d_name) + 2);

		if (fullpath == NULL) {

			perror("Ошибка выделения памяти");
			exit(EXIT_FAILURE);

		}

		snprintf(fullpath, strlen(path) + strlen(entry->d_name) + 2, "%s/%s", path, entry->d_name);

		if (entry->d_type == DT_UNKNOWN) {

			if (lstat(fullpath, statbuf) != 0) {

				perror("Ошибка вызова stat");
				exit(EXIT_FAILURE);

			}

			if (S_ISREG(statbuf->st_mode)) {

				(*entries)[*count] = fullpath;
				(*count)++;

				if (*count >= *capacity * 0.7) {
					memInc(entries, capacity);
				}

			} else if (S_ISDIR(statbuf->st_mode)) {

				walkdirL(fullpath, entries, count, capacity);
				free(fullpath);

			} else {

				free(fullpath);

			}

		} else if (entry->d_type == DT_REG) {

			(*entries)[*count] = fullpath;
			(*count)++;

			if (*count >= *capacity * 0.7) {

				memInc(entries, capacity);

			}

		} else if (entry->d_type == DT_DIR) {

			walkdirF(fullpath, entries, count, capacity);
			free(fullpath);

		} else {

			free(fullpath);

		}

	}

    if (closedir(dir) != 0) {

        perror("Ошибка при закрытии каталога");
        return;

    }
}

void memInc(char*** entries, int* sizeArr) {
    int newSize = *sizeArr * 2;

    char** newEntries = realloc(*entries, newSize * sizeof(char*));

	if (newEntries == NULL) {
        perror("realloc failed");
        return;
    }

    *entries = newEntries;
    *sizeArr = newSize;
}

void options(int* argc, char*** argv, int* flag) {

	int opt;

	while ((opt = getopt(*argc, *argv, "ldfs")) != -1 ){
		switch (opt) {
			case 'l' : 
				*flag|= FLAG_L;
				break;
			case 'd' :
				*flag|= FLAG_D;
				break;
			case 'f' :
				*flag|= FLAG_F;
				break;
			case 's' :
				*flag|= FLAG_S;
				break;
			default:
				fprintf(stderr, "Usage: %s [-l] [-d] [-f] [-s]\n", (*argv)[0]);
				exit(EXIT_FAILURE);
		}
	}
}

void pathfind(int* argc, char*** argv, char** path){

	*path = NULL;

	for (int i = 1; i < *argc; i++) {
        if ((*argv)[i][0] != '-') {
            *path = (*argv)[i];
            break;
        }
    }
	if (!(*path)) {
		*path = ".";
	}
}

int compare(const void *a, const void *b) {
    return strcoll(*(const char **)a, *(const char **)b);
}
