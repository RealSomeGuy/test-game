#include "utils.h"
#include <stdlib.h>

char *read_file(FILE *file)
{
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	char *fstr = malloc(size + 1);

	fread(fstr, 1, size, file);
	fstr[size] = '\0';

	return fstr;
}

