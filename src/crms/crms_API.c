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
    char* buffer_frame= calloc(BUFFER_FRAME,sizeof(char)); //NO SE USA
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
        pecebe->id = buffer_tabla[i+1];
        pecebe->nombre = calloc(12,sizeof(char));
        for (int j = i+2; j < i+14; j++)
        {
            pecebe->nombre[contador_nombre] = buffer_tabla[j];
            contador_nombre += 1;
        }
        printf("%s \n",pecebe->nombre);        
        CrmsFile** subentradas = calloc(10,sizeof(CrmsFile*));
        
        for (int j = 0; j < 10; j++)
        {
            int contador_nombre_subentrada = 0;
            CrmsFile* subentrada = malloc(sizeof(CrmsFile));
            subentrada->validez = buffer_tabla[i+14];
            //printf("validez %i\n", subentrada->validez);
            subentrada->nombre = calloc(12,sizeof(char));
            for (int k = (i+15) + (21*j); k < i+27 + (21*j); k++)
            {
                subentrada->nombre[contador_nombre_subentrada] = buffer_tabla[k];
                contador_nombre_subentrada += 1;
            }
            if (subentrada->nombre)
            {
                //printf("%s\n", subentrada->nombre);
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
            
            unsigned int numero = 496; 
            unsigned int numero_offset = 4294966784;

            unsigned int vpn = (direccion & numero) >> 4;
            unsigned int offset = (direccion & numero_offset) >> 9;
            
            subentrada->offset = offset;
            subentrada->vpn = vpn;

            subentradas[j] = subentrada;
        }
        pecebe->subentradas = subentradas;
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
        printf("byte: %u\n", byte);
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
        printf("Bin: %i %i %i %i     %i %i %i %i \n", primero, segundo, tercero , cuarto, quinto, sexto, septimo, octavo);


    }
    
    crms->bitmap = bitmap;
    crms->tabla_pcb = tabla_pcb; 
    //TO DO: crear variable global

}

CrmsFile* cr_open(int process_id, char* file_name, char mode){
    CrmsFile* crmsfile;
    if (mode == 'r'){
        //do stuff
        //crmsfile.process_id = process_id;
        for (int i = 0; i < 16; i++){ //reviso los id de todos los procesos
            if (crms->tabla_pcb[i]->id == process_id){
                for (int j = 0; j < 10; j++){ //reviso los nombres de los archivos del proceso
                    if (crms->tabla_pcb[i]->subentradas[j]->validez){
                        if (crms->tabla_pcb[i]->subentradas[j]->name == file_name){ //agregar validez de subentrada.
                            crmsfile = crms->tabla_pcb[i]->subentradas[j];
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    if (mode == 'w'){
        //do other stuff
        if(cr_exists(process_id, file_name)) 
        {
            crmsfile = NULL;
        }
        else{
            crmsfile = calloc(1, sizeof(CrmsFile));
            crmsfile->nombre = calloc(12,sizeof(char));
            crmsfile->nombre = file_name;
            //crmsfile->process_id = process_id;
            crmsfile->tamano = 0;
            //to do: poner en una subentrada o ponerlo al escribir
        }
    }
    return crmsfile;
}

int cr_write_file(CrmsFile* file_desc, char* buffer, int n_bytes){
    //encontrar el primer lugar vacio.
    int first_vpn = 0;
    int first_offset = 0;
    int second_vpn = 32;//para ver hasta donde puedo escribir.
    int second_offset = BUFFER_PAGINA-1; //8MB
    for (int i = 0; i < 16; i++){ //reviso los id de todos los procesos |    |
        if (crms->tabla_pcb[i]->id == file_desc->process_id){
            int estado = 1;
            while (estado){
                estado = 0;
                for (int j = 0; j < 10; j++){
                    if (crms->tabla_pcb[i]->subentradas[j]->validez){
                        if (first_vpn == crms->tabla_pcb[i]->subentradas[j]->vpn){
                            if (first_offset == crms->tabla_pcb[i]->subentradas[j]->offset){
                                //sumar tamaño y cambiar first_vpn y first offset
                                first_vpn = crms->tabla_pcb[i]->subentradas[j]->vpn;
                                first_offset = crms->tabla_pcb[i]->subentradas[j]->offset;
                                first_offset += crms->tabla_pcb[i]->subentradas[j]->tamaño;
                                while (first_offset > (BUFFER_PAGINA-1)){ //REVISAR CASOS BORDE 
                                    first_vpn += 1;
                                    first_offset -= BUFFER_PAGINA; //REVISAR CASOS BORDE le resto el tamaño de la pagina para dezplazar el offset
                                }
                                estado = 1;
                            }
                        }
                    }
                }
            }
            int second_vpn = 32;//para ver hasta donde puedo escribir.
            int second_offset = BUFFER_PAGINA-1; //8MB
            for (int j = 0; j < 10; j++){
                if (crms->tabla_pcb[i]->subentradas[j]->validez){
                    if (first_vpn == second_vpn){
                        if (first_vpn == crms->tabla_pcb[i]->subentradas[j]->vpn){
                            if ((first_offset < crms->tabla_pcb[i]->subentradas[j]->offset) && (crms->tabla_pcb[i]->subentradas[j]->offset <= second_offset)){
                                second_offset = crms->tabla_pcb[i]->subentradas[j]->offset;
                            }
                        }
                    }
                    else{
                        if (second_vpn == crms->tabla_pcb[i]->subentradas[j]-> vpn){//revisar el rango del vpn
                            if (second_offset > crms->tabla_pcb[i]->subentradas[j]->offset){
                                second_offset == crms->tabla_pcb[i]->subentradas[j]->offset;
                            }
                        }
                        else if ((second_vpn > crms->tabla_pcb[i]->subentradas[j]->vpn) && (first_vpn == crms->tabla_pcb[i]->subentradas[j]->vpn)){
                            if (first_offset < crms->tabla_pcb[i]->subentradas[j]->offset){
                                second_offset == crms->tabla_pcb[i]->subentradas[j]->offset;
                            }
                        }
                        else if ((second_vpn > crms->tabla_pcb[i]->subentradas[j]->vpn) && (first_vpn < crms->tabla_pcb[i]->subentradas[j]->vpn)){
                            second_vpn = crms->tabla_pcb[i]->subentradas[j]->vpn;
                            second_offset = crms->tabla_pcb[i]->subentradas[j]->offset;
                        }
                    }
                }
            }
            break;
        }
    }
    //Por hacer: Escritura. Y poner en la tabla de archivos PD: NUNCA SE ESCRIBE EN second.
    
    int espacio_libre = 0; //calcular tamaño del espacio libre.
    if (first_vpn == second_vpn){
        espacio_libre = second_offset - first_offset;
    }
    else{
        espacio_libre += BUFFER_PAGINA - first_offset; //tamaño pagina - first offset
        espacio_libre += (second_vpn - first_vpn - 1)*BUFFER_PAGINA; //tamaño de paginas entre incio y fin
        espacio_libre += second_offset; //second_offset
    }
    //Por hacer: buscar espacio de memoria fisica.
    int espacio_libre;
    char* file_name = crms->mem_file;
    FILE* file_pointer = fopen(filename, "rb+");
    
    int desplazamiento_pointer = BUFFER_BITMAP + BUFFER_TABLA; //sumar direccion de memoria fisica
    fseek(file_pointer, desplazamiento_pointer, SEEK_SET); 
    int counter = 0;
    while (n_bytes && espacio_libre){
        //fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream); //prt direccion de origen, size tamaño de bytes a escribir, nmemb numero de elementos, stream es el file
        fwrite(buffer, 1, 1, file_pointer);
        fseek(file_pointer, 1, SEEK_SET); 
        //REVISAR CAMBIO DE FRAME
        n_bytes --;
        espacio_libre --;
    }
}
