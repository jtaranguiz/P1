#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include "crms_API.h"
int main(int argc, char **argv)
{
    char* filename = argv[1];
    printf("%s",filename);
    printf("FUNCIONA! \n");
    cr_mount(filename);
    return 0;
}