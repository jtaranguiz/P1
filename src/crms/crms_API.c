#include <stdio.h>  // FILE, fopen, fclose, etc.
#include <string.h> // strtok, strcpy, etc.
#include <stdlib.h>
#include <byteswap.h>
#include "crms_API.h"
#include <ctype.h>
extern char* ruta;
extern Crms* crms;
void cr_mount(char* filename)
{
    ruta = filename;
    printf("%s\n",ruta);
}
Crms* asignar (char* filename)
{
    FILE* file_pointer = fopen(filename, "rb");
    char* buffer_tabla = calloc(BUFFER_TABLA, sizeof(char));
    char* buffer_bitmap = calloc(BUFFER_BITMAP,sizeof(char));
    char* buffer_frame= calloc(BUFFER_FRAME,sizeof(char));
    printf("en asignar: %s", filename);
    //liberar memoria
    int contador_pcbs = 0;
    Crms* crms = malloc(sizeof(Crms));
    fread(buffer_tabla,sizeof(char),BUFFER_TABLA,file_pointer);
    printf("antes del for \n");
    Pcb** tabla_pcb = calloc (16, sizeof(Pcb*)); 
    
    for (int i = 0; i < BUFFER_TABLA; i+=256)
    {
        int contador_nombre = 0;
        Pcb* pecebe = malloc(sizeof(Pcb));
        pecebe->estado = buffer_tabla[i];
        pecebe->id = (unsigned int)((unsigned char)buffer_tabla[i+1]);
        pecebe->nombre = calloc(12,sizeof(char));
        for (int j = i+2; j < i+14; j++)
        {
            pecebe->nombre[contador_nombre] = buffer_tabla[j];
            contador_nombre += 1;
        }
        printf("%s \n",pecebe->nombre); 
        pecebe->subentradas = calloc(10,sizeof(CrmsFile*));    
        //CrmsFile** subentradas = calloc(10,sizeof(CrmsFile*));
        
        for (int j = 0; j < 10; j++)
        {
            int contador_nombre_subentrada = 0;
            CrmsFile* subentrada = malloc(sizeof(CrmsFile));
            subentrada->validez = buffer_tabla[i+14 + (21*j)];
            //printf("validez %i\n", subentrada->validez);
            subentrada->nombre = calloc(12,sizeof(char));
            for (int k = (i+15) + (21*j); k < i+27 + (21*j); k++)
            {
                subentrada->nombre[contador_nombre_subentrada] = buffer_tabla[k];
                contador_nombre_subentrada += 1;
            }
            if (subentrada->nombre)
            {
                printf("%s\n", subentrada->nombre);
            }
            
            
            //falta comprobar que este en big endian preguntar sino
            unsigned int * tamano = calloc(4,sizeof(char));
            // primero ponemos como unsigned char al buffer, luego lo asginamos a un unsigned int
            // Swapeamos el valor del byte
            // shifts a la posicion correspondiente
            // lo asignamos a un unsigned int que es la suma de todos.
            
            unsigned int primero =  bswap_32((unsigned int)((unsigned char) buffer_tabla[i+27 + (21*j)])); 
            unsigned int segundo =  bswap_32((unsigned int)((unsigned char)buffer_tabla[i+28 + (21*j)]));
            unsigned int tercero =  bswap_32((unsigned int)((unsigned char)buffer_tabla[i+29 + (21*j)]));
            unsigned int cuarto =  bswap_32((unsigned int) ((unsigned char)buffer_tabla[i+30 + (21*j)]));
            
            unsigned int suma = 0;
            cuarto = cuarto >> 24;  
            tercero = tercero >> 16;
            segundo = segundo >> 8; 
            suma = primero | segundo | tercero | cuarto;
            subentrada->tamano = suma;
            
            
                  
            subentrada->vpn_offset = calloc(4,sizeof(char)); //ESTO NO SE USA
            //luego separar el vpn y offset
            //int contador_direccion = 3;
            unsigned int direccion = 0;
            unsigned int dir_primero = bswap_32((unsigned int)((unsigned char)buffer_tabla[i+31 + (21*j)]));
            unsigned int dir_segundo = bswap_32((unsigned int)((unsigned char)buffer_tabla[i+32 + (21*j)]));
            unsigned int dir_tercero = bswap_32((unsigned int)((unsigned char)buffer_tabla[i+33 + (21*j)]));
            unsigned int dir_cuarto = bswap_32((unsigned int)((unsigned char)buffer_tabla[i+34 + (21*j)]));

            dir_cuarto = dir_cuarto >> 24; 
            dir_tercero = dir_tercero >> 16;
            dir_segundo = dir_segundo >> 8; 
            
            direccion = dir_primero | dir_segundo | dir_tercero | dir_cuarto;
            printf("DIRECCiON :%u \n",direccion);
            unsigned int numero = 496; 
            unsigned int numero_offset = 4294966784;

            unsigned int vpn = (direccion & numero) >> 4;
            unsigned int offset = (direccion & numero_offset) >> 9;
            
            subentrada->offset = offset;
            subentrada->vpn = vpn;

            pecebe->subentradas[j] = subentrada;
        }
        //pecebe->subentradas = subentradas;
        int contador_pag = 0;
        unsigned int info;
        pecebe->tablapag.paginas = calloc(32, sizeof(Pagina*));
        for (int j = i+224; j < i+256 ; j++)
        {
            Pagina* pagina = malloc(sizeof(Pagina));
            info = (unsigned int)((unsigned char)buffer_tabla[j]); //uno de los bits es la validez, separar
            //info = info >> 24;
            unsigned int validez = info & 128 >> 7;
            unsigned int pfn = ((info & 127));
            //printf("validez:%u pfn:%u \n",validez, pfn);
            pagina->pfn = pfn;
            pagina->validez = validez;
            pecebe->tablapag.paginas[contador_pag] = pagina;
            contador_pag += 1;
        }
        tabla_pcb[contador_pcbs] = pecebe;
        contador_pcbs += 1;   
    }
    
    fseek(file_pointer, BUFFER_TABLA,SEEK_SET); //FUNCIONA?????
    fread(buffer_bitmap, 1, BUFFER_BITMAP, file_pointer);
    int contador_bitmap = 0; 
    Bitmap* bitmap = malloc(sizeof(Bitmap));
    bitmap->arreglo = calloc(128, sizeof(int));
    for (int i = 0; i < BUFFER_BITMAP; i++)
    {

        unsigned int byte = (unsigned int)((unsigned char)buffer_bitmap[i]);
        int primero = (byte & 1);
        bitmap->arreglo[contador_bitmap] = primero;
        contador_bitmap += 1;
        int segundo = (byte & 2) >> 1;
        bitmap->arreglo[contador_bitmap] = segundo;
        contador_bitmap += 1;
        int tercero = (byte & 4) >> 2;
        bitmap->arreglo[contador_bitmap] = tercero;
        contador_bitmap += 1;
        int cuarto = (byte & 8) >> 3 ;
        bitmap->arreglo[contador_bitmap] = cuarto;
        contador_bitmap += 1;
        int quinto = (byte & 16) >> 4;
        bitmap->arreglo[contador_bitmap] = quinto;
        contador_bitmap += 1;
        int sexto = (byte & 32) >> 5;
        bitmap->arreglo[contador_bitmap] = sexto;
        contador_bitmap += 1;
        int septimo = (byte & 64) >> 6;
        bitmap->arreglo[contador_bitmap] = septimo;
        contador_bitmap += 1;
        int octavo = (byte & 128) >> 7;
        bitmap->arreglo[contador_bitmap] = octavo;
        contador_bitmap += 1;
        //printf("Bin: %i %i %i %i     %i %i %i %i \n", primero, segundo, tercero , cuarto, quinto, sexto, septimo, octavo);

        
    }
    
    crms->bitmap = bitmap;
    crms->tabla_pcb = tabla_pcb;
    return crms;

}
void cr_ls_processes()
{
    printf("Imprimiendo procesos en ejecucion \n");
    for (int i = 0; i < 16; i++)
    {
        Pcb* proceso = crms->tabla_pcb[i];
        if (proceso->estado)
        {
            printf("id: %u, nombre: %s\n",proceso->id,proceso->nombre);
        }
        
    }
    
}
int cr_exists(unsigned int process_id, char* filename)
{
    
    for (int i = 0; i < 16; i++)
    {
        Pcb* proceso = crms->tabla_pcb[i];
        if ((proceso->id == process_id) && (proceso->estado==1))
        {
            printf("entre por el id\n");
            for (int j = 0; j < 10; j++)
            {
                if (strcmp(proceso->subentradas[j]->nombre, filename) == 0)
                {
                    printf("entre por el nombre\n");
                    return 1;
                }
                
            }
            printf("no encontre el nombre\n");
            return 0;
            
        }
        
        
    }
    printf("no encontre por el id\n");
    return 0;
    
}
void cr_ls_files(int process_id)
{
    printf("Imprimiendo archivos del proceso %i\n",process_id);
    for (int i = 0; i < 16; i++)
    {
        //printf("||||||||||||||||||||||||||\n");
        Pcb* proceso = crms->tabla_pcb[i];
        if (proceso->id == process_id )
        {
            for (int j = 0; j < 10; j++)
            {
                //printf("NOMBRE:%s,VALIDEZ:%i\n",proceso->subentradas[j]->nombre,proceso->subentradas[j]->validez);
                if (proceso->subentradas[j]->validez)
                {
                    if (proceso->subentradas[j]->nombre)
                    {
                        printf("%s\n",proceso->subentradas[j]->nombre);
                    }
                }
            }
            
        }
        
    }    
}

void cr_start_process(int process_id, char* process_name)
{
    int parada = -1;
    for (int i = 0; i < 16; i++)
    {
        Pcb* proceso = crms->tabla_pcb[i];
        if (proceso->estado==0)
        {
            parada = i;
            break;
        }   
    }
    if (parada != -1)
    {
        crms->tabla_pcb[parada]->id = process_id;
        crms->tabla_pcb[parada]->nombre = process_name; 
        crms->tabla_pcb[parada]->estado = 1;
    }
}

void cr_finish_process(int process_id)
{
    for (int i = 0; i < 16; i++)
    {
        Pcb* proceso = crms->tabla_pcb[i];
        if (proceso->id == process_id)
        {
           for (int j = 0; j < 10; j++)
           {
               proceso->subentradas[j]->validez = 0;
           }
           proceso->estado = 0;
        }
        
    }
}
