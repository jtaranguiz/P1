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
    cr_ls_processes();//procesos despues de crear uno nuevo
    cr_ls_files(0);
    CrmsFile* crms_file = cr_open(0, "secret.txt", 'r');
    int existe = cr_exists(0, "secret.txt");
    
    printf("existe: %i\n", existe);
    printf("tamaño: %i\n", crms_file->tamano);
    char* buffer2 = calloc(crms_file->tamano, sizeof(char));
    printf("buffer: \n");
    int escrito = cr_read(crms_file, buffer2, 1000);
    for (int i = 0; i<escrito; i++){
    printf("%c", buffer2[i]);
    }
    cr_close(crms_file);
    crms_file = cr_open(0, "im_a_mp3.txt", 'r');
    printf("VPN: %u, OFFSET: %u \n", crms_file->vpn,crms_file->offset);
    printf("\n");
    free(buffer2);
    printf("Frame Bitmap:\n");
    for (int i = 0; i<128; i++){
        printf("%i, ", crms->bitmap->arreglo[i]);
        if ((i+1)%8==0){
            printf("\n");
        }
    }
    printf("Borramos el archivo message.txt...\n");
    cr_delete_file(crms_file);
    cr_ls_files(0);
    printf("Frame Bitmap:\n");
    for (int i = 0; i<128; i++){
        printf("%i, ", crms->bitmap->arreglo[i]);
        if ((i+1)%8==0){
            printf("\n");
        }
    }
    printf("guardamos en memoria...\n");    
    close_memory();
}