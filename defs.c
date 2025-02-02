#include "main.h"
FILE *file;
int flags = 0;
bool read_walls = false;
warr *walls = NULL;
command_breakdown commandb;

struct tm *prevtime = NULL;
