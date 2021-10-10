#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include <byteswap.h>
#include "crms_API.h"
void cr_mount (char* filename)
{
    FILE* file_pointer = fopen(filename, "rb");
    char* buffer_tabla = calloc(BUFFER_TABLA, sizeof(char));
    char* buffer_bitmap = calloc(BUFFER_BITMAP,sizeof(char));
    char* buffer_frame= calloc(BUFFER_FRAME,sizeof(char));
    //liberar memoria
    fread(buffer_tabla,sizeof(char),BUFFER_TABLA,file_pointer);
    printf("antes del for \n");
    Pcb** tabla_pcb = calloc (16, sizeof(Pcb*));
    for (int i = 0; i < BUFFER_TABLA; i+=256)
    {
        int contador_nombre = 0;
        Pcb* pecebe = malloc(sizeof(Pcb));
        pecebe->estado = buffer_tabla[i];
        pecebe->id = buffer_tabla[i+1];
        pecebe->nombre = calloc(12,sizeof(char));
        for (int j = i+2; j < i+14; j++)
        {
            pecebe->nombre[contador_nombre] = buffer_tabla[j];
            contador_nombre += 1;
        }
        printf("%s \n",pecebe->nombre);        
        Subentrada** subentradas = calloc(10,sizeof(Subentrada));
        
        for (int j = 0; j < 10; j++)
        {
            int contador_nombre_subentrada = 0;
            Subentrada* subentrada = malloc(sizeof(Subentrada));
            subentrada->validez = buffer_tabla[i+14];
            printf("validez %i\n", subentrada->validez);
            subentrada->nombre = calloc(12,sizeof(char));
            for (int k = (i+15) + (21*j); k < i+27 + (21*j); k++)
            {
                subentrada->nombre[contador_nombre_subentrada] = buffer_tabla[k];
                contador_nombre_subentrada += 1;
            }
            printf("%s\n", subentrada->nombre);
            //falta comprobar que este en big endian preguntar sino
            int primero = (int) buffer_tabla[i+30 + (21*j)] ; 
            int segundo = (int) buffer_tabla[i+29 + (21*j)];
            int tercero_2 =  buffer_tabla[i+28 + (21*j)];
            printf("char tercero: %c\n", buffer_tabla[i+28 + (21*j)]);
            int tercero = (int) buffer_tabla[i+28 + (21*j)];
            int cuarto = (int) buffer_tabla[i+27 + (21*j)];
            //printf("cuarto: %u \n", cuarto);
            cuarto = cuarto << 24;
            //printf("cuarto: %u \n", cuarto);
            //printf("tercero: %u\n",  tercero);
            tercero = tercero << 16;
            //printf("tercero: %u\n",  tercero);
            //printf("segundo %u\n",  segundo);
            segundo = segundo << 8;
            //printf("segundo %u\n",  segundo);
            //printf("primero %u\n",  primero);
            unsigned int juntos = primero | segundo | tercero | cuarto ;
            subentrada->tamano = juntos;
            //printf("tamano: %u\n", subentrada->tamano);      
            subentrada->vpn_offset = calloc(4,sizeof(char));
            //luego separar el vpn y offset
            int contador_direccion = 3;
            for (int k = i+31 + (21*j); k <= i+34 + (21*j); k++)
            {
                subentrada->vpn_offset[contador_direccion] = buffer_tabla[k];
                contador_direccion -= 1;
            } 
            subentradas[j] = subentrada;
        }
        int contador_pag = 0;
        pecebe->tablapag = calloc(32, sizeof(char));
        for (int j = i+35; j < i+67 ; j++)
        {
            pecebe->tablapag[contador_pag] = buffer_tabla[j]; //uno de los bits es la validez, separar
            contador_pag += 1;
        }
        
        
    }
    
    fseek(file_pointer, BUFFER_TABLA,SEEK_SET);

    

    fread(buffer_bitmap, BUFFER_BITMAP,1, file_pointer);
    fseek(file_pointer, BUFFER_BITMAP, SEEK_SET + BUFFER_TABLA);

    fread(buffer_frame,BUFFER_FRAME,1,file_pointer);
    fseek(file_pointer, BUFFER_FRAME, SEEK_SET + BUFFER_TABLA + BUFFER_BITMAP);

}