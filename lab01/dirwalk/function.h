#include <stdio.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <sys/stat.h>
#define CAPACITY 10
#define FLAG_L (1 << 0)
#define FLAG_D (1 << 1)
#define FLAG_F (1 << 2)
#define FLAG_S (1 << 3)

void walkdir(char* path, char*** entries, int* count, int* capacity);

void walkdirL(char* path, char*** entries, int* count, int* capacity);

void walkdirD(char* path, char*** entries, int* count, int* capacity);

void walkdirF(char* path, char*** entries, int* count, int* capacity);

void memInc(char*** entries, int* sizeArr);

void options(int*, char***, int*);

void pathfind(int* argc, char*** argv, char** path);

int compare(const void *a, const void *b);
