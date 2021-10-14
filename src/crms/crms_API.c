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
            
            
                  
            //ESTO NO SE USA
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
            
            unsigned int numero = 260046848; 
            unsigned int numero_offset = 8388607;

            unsigned int vpn = (direccion & numero) >> 23;
            unsigned int offset = (direccion & numero_offset);
            subentrada->vpn_offset = direccion;
            subentrada->offset = offset;
            subentrada->vpn = vpn;
            subentrada ->process = pecebe;
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
            printf("id: %u, nombre: %s, estado: %i\n",proceso->id,proceso->nombre, proceso->estado);
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
        Pcb* proceso = crms->tabla_pcb[i];
        if (proceso->id == process_id)
        {
            for (int j = 0; j < 10; j++)
            {
                if (proceso->subentradas[j]->validez)
                {
                    printf("%s\n",proceso->subentradas[j]->nombre);
                }
            }
            
        }
        
    }
}


CrmsFile* cr_open(int process_id, char* file_name, char mode){
    CrmsFile* crmsfile;
    if (mode == 'r'){
        for (int i = 0; i < 16; i++){ //reviso los id de todos los procesos
            if ((crms->tabla_pcb[i]->id == process_id) && crms->tabla_pcb[i]->estado){
                //printf("id_process, name: %u, %s\n", crms->tabla_pcb[i]->id, crms->tabla_pcb[i]->nombre);
                for (int j = 0; j < 10; j++){ //reviso los nombres de los archivos del proceso
                    if (crms->tabla_pcb[i]->subentradas[j]->validez){
                        //printf("archivo, nombre, iguales: %i, %s\n", crms->tabla_pcb[i]->subentradas[j]->validez, crms->tabla_pcb[i]->subentradas[j]->nombre);
                        if (strcmp(crms->tabla_pcb[i]->subentradas[j]->nombre, file_name) == 0){ 
                            crmsfile = crms->tabla_pcb[i]->subentradas[j];
                            //printf("nombre_archivo_open: %s\n", crmsfile->nombre);
                            //printf("nombre processo: %s\n", crmsfile->process->nombre);
                            //printf("validez: %i\n", crmsfile->validez);
                            //printf("tamano: %i\n", crmsfile->tamano);
                            break;
                        }
                    }
                }
                break;
            }
        }
    }
    if (mode == 'w'){
        if(cr_exists(process_id, file_name)) 
        {
            //printf("ya existe \n");
            crmsfile = NULL;
        }
        else{
            crmsfile = calloc(1, sizeof(CrmsFile));
            crmsfile->nombre = calloc(12,sizeof(char));
            crmsfile->nombre = file_name;
            crmsfile->tamano = 0;
            for (int i = 0; i < 16; i++){ //reviso los id de todos los procesos
                if ((crms->tabla_pcb[i]->id == process_id) && crms->tabla_pcb[i]->estado){
                    crmsfile->process = crms->tabla_pcb[i];
                    //printf("id_process, name: %u, %s\n", crms->tabla_pcb[i]->id, crms->tabla_pcb[i]->nombre);
                }
            }
        }
    }
    return crmsfile;
}

unsigned int obtener_dir(unsigned int vpn, unsigned int offset){
    vpn = vpn << 23;
    
    unsigned int dir = vpn | offset;
    return dir;
}

unsigned int* find_first_empty(Pcb* process,unsigned int first_vpn,unsigned int first_offset){
    unsigned int* dir_pointer = calloc(2, sizeof(unsigned int));
    int estado = 1;
    while (estado){
        estado = 0;
        for (int j = 0; j < 10; j++){
            if (process->subentradas[j]->validez){
                if (first_vpn == process->subentradas[j]->vpn){
                    if (first_offset == process->subentradas[j]->offset){
                        //sumar tamano y cambiar first_vpn y first offset
                        first_vpn = process->subentradas[j]->vpn;
                        first_offset = process->subentradas[j]->offset;
                        first_offset += process->subentradas[j]->tamano;
                        while (first_offset > (BUFFER_PAGINA-1)){ //REVISAR CASOS BORDE 
                            first_vpn += 1;
                            first_offset -= BUFFER_PAGINA; //REVISAR CASOS BORDE le resto el tamano de la pagina para dezplazar el offset
                        }
                        estado = 1;
                    }
                }
            }
        }
    }
    dir_pointer[0] = first_vpn;
    dir_pointer[1] = first_offset;
    return dir_pointer;
}

unsigned int* find_final_empty(Pcb* process,unsigned int first_vpn,unsigned int first_offset, unsigned int second_vpn, unsigned int second_offset){
    unsigned int* dir_pointer = calloc(2, sizeof(unsigned int));
    for (int j = 0; j < 10; j++){
        if (process->subentradas[j]->validez){
            if (first_vpn == second_vpn){
                if (first_vpn == process->subentradas[j]->vpn){
                    if ((first_offset < process->subentradas[j]->offset) && (process->subentradas[j]->offset <= second_offset)){
                        second_offset = process->subentradas[j]->offset;
                    }
                }
            }
            else{
                if (second_vpn == process->subentradas[j]-> vpn){//revisar el rango del vpn
                    if (second_offset > process->subentradas[j]->offset){
                        second_offset = process->subentradas[j]->offset;
                    }
                }
                else if ((second_vpn > process->subentradas[j]->vpn) && (first_vpn == process->subentradas[j]->vpn)){
                    if (first_offset < process->subentradas[j]->offset){
                        second_offset = process->subentradas[j]->offset;
                    }
                }
                else if ((second_vpn > process->subentradas[j]->vpn) && (first_vpn < process->subentradas[j]->vpn)){
                    second_vpn = process->subentradas[j]->vpn;
                    second_offset = process->subentradas[j]->offset;
                }
            }
        }
    }
    dir_pointer[0] = second_vpn;
    dir_pointer[1] = second_offset;
    return dir_pointer;
}

int find_tamano_libre(unsigned int first_vpn, unsigned int first_offset, unsigned int second_vpn, unsigned int second_offset){
    int espacio_libre = 0;
    if (first_vpn == second_vpn){
        espacio_libre = second_offset - first_offset;
    }
    else{
        espacio_libre += BUFFER_PAGINA - first_offset; //tamano pagina - first offset
        espacio_libre += (second_vpn - first_vpn - 1)*BUFFER_PAGINA; //tamano de paginas entre incio y fin
        espacio_libre += second_offset; //second_offset
    }
    return espacio_libre;
}

int cr_write_file(CrmsFile* file_desc, char* buffer, int n_bytes){
    //encontrar el primer lugar vacio.
    unsigned int first_vpn = 0;
    unsigned int first_offset = 0;
    unsigned int second_vpn = 32;//para ver hasta donde puedo escribir.
    unsigned int second_offset = BUFFER_PAGINA; //8MB
    unsigned int pfn = 0;
    Pcb* process = file_desc->process; //reviso los id de todos los procesos 
    unsigned int* dir_pointer = find_first_empty(process, first_vpn, first_offset); //Funcion que retorna el puntero a dos valores con el inicio
    first_vpn = dir_pointer[0];
    first_offset = dir_pointer[1];
    free(dir_pointer);
    dir_pointer = find_final_empty(process, first_vpn, first_offset, second_vpn, second_offset); //Funcion que retorna el final 
    second_vpn = dir_pointer[0];
    second_offset = dir_pointer[1];
    free(dir_pointer);
    for (int j = 0; j < 10; j++){ // update subentrada
        if (process->subentradas[j]->validez == 0){
            file_desc->vpn = first_vpn;
            file_desc->offset = first_offset;
            file_desc->validez = 1;
            process->subentradas[j] = file_desc;
            break;
        }
    }
    int disp_frame = 0; //TRADUCIR A funcion
    if (process->tablapag.paginas[first_vpn]->validez){
        pfn= process->tablapag.paginas[first_vpn]->pfn;
        disp_frame = 1;
    }
    else{
        for (int j = 0; j < 128; j++){
            if (crms->bitmap->arreglo[j] == 0){
                pfn = j;
                process->tablapag.paginas[first_vpn]->validez = 1;
                process->tablapag.paginas[first_vpn]->pfn = pfn;
                disp_frame = 1;
                break;
            }
        }
    }

    unsigned int dir_fisica = obtener_dir(pfn, first_offset); //espacio fisico
    int espacio_libre = 0; //calcular tamano del espacio libre.
    espacio_libre = find_tamano_libre(first_vpn, first_offset, second_vpn, second_offset);
    FILE* file_pointer = fopen(ruta, "rb+");
    
    int desplazamiento_pointer = BUFFER_BITMAP + BUFFER_TABLA + dir_fisica; //sumar direccion de memoria fisica
    fseek(file_pointer, desplazamiento_pointer, SEEK_SET); 
    int counter = 0; //revisar si se necesita
    int contador_char = 0;
    char* caracter;
    printf("tamano: %u\n", espacio_libre);
    while (n_bytes && espacio_libre && disp_frame){ //Escritura
        //fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream); //prt direccion de origen, size tamano de bytes a escribir, nmemb numero de elementos, stream es el file
        caracter = &buffer[contador_char];
        fwrite(caracter, 1, 1, file_pointer); 
        //REVISAR CAMBIO DE FRAME
        first_offset ++;
        counter ++;
        contador_char ++;
        if (first_offset > (BUFFER_PAGINA -1)){
            first_offset = 0;
            first_vpn ++;
            disp_frame = 0; //TRADUCIR A funcion
            if (process->tablapag.paginas[first_vpn]->validez){
                pfn= process->tablapag.paginas[first_vpn]->pfn;
                disp_frame = 1;
            }
            else{
                for (int j = 0; j < 128; j++){
                    if (crms->bitmap->arreglo[j] == 0){
                        pfn = j;
                        process->tablapag.paginas[first_vpn]->validez = 1;
                        process->tablapag.paginas[first_vpn]->pfn = pfn;
                        disp_frame = 1;
                        break;
                    }
                }
            }
            dir_fisica = obtener_dir(pfn, first_offset);
            if (disp_frame){
                desplazamiento_pointer = BUFFER_BITMAP + BUFFER_TABLA + dir_fisica;
                fseek(file_pointer, desplazamiento_pointer, SEEK_SET);
            }
            counter = 0;
        }
        n_bytes --;
        espacio_libre --;
    }
    fclose(file_pointer);
    if (disp_frame == 0){
        file_desc->tamano = 0;
        return 0;
    }
    else{
        file_desc->tamano = contador_char;
        return contador_char;
    }
}

int cr_read(CrmsFile* file_desc, char* buffer, int n_bytes){
    Pcb* process = file_desc->process;
    unsigned int pfn = 0;
    unsigned int tamano = file_desc->tamano;
    unsigned int offset = file_desc->offset;
    unsigned int vpn = file_desc->vpn;
    if (process->tablapag.paginas[vpn]->validez){
        printf("in validez\n");
        pfn = process->tablapag.paginas[vpn]->pfn;
    }
    unsigned int dir_fisica = obtener_dir(pfn, offset);
    FILE* file_pointer = fopen(ruta, "rb");
    printf("vpn = %u\n", vpn);
    int desplazamiento_pointer = BUFFER_BITMAP + BUFFER_TABLA + dir_fisica; //sumar direccion de memoria fisica
    fseek(file_pointer, desplazamiento_pointer, SEEK_SET); 
    int contador_char = 0;
    char* caracter;
    while(n_bytes && tamano){
        caracter = &buffer[contador_char];
        fread(caracter, sizeof(char), 1, file_pointer);
        printf("%c", *caracter);
        contador_char ++;
        tamano --;
        n_bytes --;
        offset ++;
        if (offset > (BUFFER_PAGINA -1)){
            offset = 0;
            vpn ++;
            if (process->tablapag.paginas[vpn]->validez){
                pfn = process->tablapag.paginas[vpn]->pfn;
            }
            dir_fisica = obtener_dir(pfn, offset);
            desplazamiento_pointer = BUFFER_BITMAP + BUFFER_TABLA + dir_fisica;
            fseek(file_pointer, desplazamiento_pointer, SEEK_SET);
        }
    }
    printf("\n");
    return contador_char;
}

void cr_close(CrmsFile* file_desc){
    file_desc = NULL;
}

void cr_delete_file(CrmsFile* file_desc){
    file_desc->validez = 0;
    unsigned int vpn = file_desc->vpn;
    unsigned int offset = file_desc->offset;
    offset += file_desc->tamano;
    liberar_frames(vpn, file_desc->process);
    while (offset > (BUFFER_PAGINA-1)){ //REVISAR CASOS BORDE 
        vpn += 1;
        liberar_frames(vpn, file_desc->process);
        offset -= BUFFER_PAGINA; //REVISAR CASOS BORDE le resto el tamano de la pagina para dezplazar el offset
    }
}

void liberar_frames(unsigned int vpn, Pcb* proceso){
    int filled = 0;
    int vpn_aux = vpn;
    for (int i = 0; i<10; i++){
        if (proceso->subentradas[i]->validez){
            int vpn_aux = proceso->subentradas[i]->vpn;
            if (vpn_aux == vpn){
                filled = 1;
            }
            unsigned int offset = proceso->subentradas[i]->offset;
            offset += proceso->subentradas[i]->tamano;
            while (offset > (BUFFER_PAGINA-1)){ //REVISAR CASOS BORDE 
                vpn_aux += 1;
                if (vpn_aux == vpn){
                    filled = 1;
                }
                offset -= BUFFER_PAGINA; //REVISAR CASOS BORDE le resto el tamano de la pagina para dezplazar el offset
            }
        }
    }
    if (filled == 0){
        proceso->tablapag.paginas[vpn]->validez = 0;
        int pfn = proceso->tablapag.paginas[vpn]->pfn;
        crms->bitmap->arreglo[pfn] = 0;
    }
}