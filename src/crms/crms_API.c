#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include "crms_API.h"
void recibir_memoria (char* filename)
{
    FILE* file_pointer = fopen(filename, "r+b");
    char buffer_tabla[BUFFER_TABLA];
    char buffer_bitmap[BUFFER_BITMAP];
    char buffer_frame[BUFFER_FRAME];

    fread(buffer_tabla,BUFFER_TABLA,1,file_pointer);
    //logica del struct asignar tabla a memoria en cada fread
    fseek(file_pointer, BUFFER_TABLA,SEEK_SET);

    

    fread(buffer_bitmap, BUFFER_BITMAP,1, file_pointer);
    fseek(file_pointer, BUFFER_BITMAP, SEEK_SET + BUFFER_TABLA);

    fread(buffer_frame,BUFFER_FRAME,1,file_pointer);
    fseek(file_pointer, BUFFER_FRAME, SEEK_SET + BUFFER_TABLA + BUFFER_BITMAP);

}