#include "function.h"

int main (int argc, char *argv[])
{
	setlocale(LC_ALL, "");

	int flags= 0;
	int capacity = CAPACITY;
	int count = 0;
	char** entries = malloc(capacity * sizeof(char*));
	char* path;

	pathfind(&argc, &argv, &path);

	options(&argc, &argv, &flags);

	if (flags& FLAG_L) {
        walkdirL(path, &entries, &count, &capacity);
    }
    else if (flags& FLAG_D) {
        walkdirD(path, &entries, &count, &capacity); 
    }
    else if (flags& FLAG_F) {
        walkdirF(path, &entries, &count, &capacity);
    }
	else {
		walkdir(path, &entries, &count, &capacity);
	}

    if (flags& FLAG_S) { 
		qsort(entries, count, sizeof(char *), compare);
    }

	for (int i = 0; i < count; i++) {
		printf("%s\n", entries[i]);
		free(entries[i]);
	}

	free(entries);

	return(0);
}
