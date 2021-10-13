#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include "crms_API.h"
char* ruta = NULL;
Crms* crms;
int main(int argc, char **argv)
{
    char* filename = argv[1];
    printf("%s",filename);
    printf("FUNCIONA! \n");
    cr_mount(filename);
    printf("main: %s\n",ruta);
    crms = asignar(ruta);   
    cr_ls_processes();
    int existe = cr_exists(0, "message.txt");
    printf("existe: %i\n",existe);
    cr_ls_files(0);
    cr_open(0, "message.txt", 'w');
    return 0;
}