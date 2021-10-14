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
    //cr_ls_files(0);
    CrmsFile* crmsfile = cr_open(0, "message.txt", 'r');
    unsigned int dir = obtener_dir(crmsfile ->vpn, crmsfile->offset);
    printf("dir: %u   %u\n",crmsfile->vpn,  crmsfile->offset);
    //char* buffer2 = calloc(11, sizeof(char));
    //buffer2 = "holi q hace";
    //cr_write_file(crmsfile, buffer2, 11);
    char* buffer = calloc(11, sizeof(char));
    cr_read(crmsfile, buffer, 16);
    printf("buffer = %s\n", buffer);
    free(buffer);
    cr_delete_file(crmsfile);
    cr_close(crmsfile);
    cr_ls_files(0);
    return 0;
}