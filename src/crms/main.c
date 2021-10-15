#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include "crms_API.h"
char* ruta = NULL;
Crms* crms;
int main(int argc, char **argv)
{
    char* filename = argv[1];
    cr_mount(filename);
    crms = asignar(ruta);   
    cr_ls_processes();
    int existe = cr_exists(0, "message.txt");
    printf("existe: %i\n",existe);
    //cr_ls_files(0);
    CrmsFile* crmsfile = cr_open(0, "message.txt", 'r');
    //unsigned int dir = obtener_dir(crmsfile ->vpn, crmsfile->offset);
    
    
    char* buffer = calloc(16, sizeof(char));
    cr_read(crmsfile, buffer, 16);
    printf("buffer = ");
    for (int i = 0; i<16; i++){
        printf("%c", buffer[i]);
    }
    printf("\n");
    free(buffer);
    //free(buffer2);
    //printf("dir: %u   %u\n",crmsfile->vpn,  crmsfile->offset);
    cr_ls_files(0);
    //cr_delete_file(crmsfile);
    cr_close(crmsfile);
    
    close_memory();
    return 0;
}