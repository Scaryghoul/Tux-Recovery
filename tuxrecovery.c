#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/statvfs.h>
#include "global.c"
#include "data_recovery.c"
#include "command.c"
#include "help.c"
#include "parse.c"
#include "mount.c"
#include "search.c"

int main(int argc, char **argv)
{
    printf("Welcome to TuxRecovery 1.5!\n");
    command(argc,argv);
}
