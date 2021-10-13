#pragma once
#define BUFFER_TABLA 4096
#define BUFFER_BITMAP 16
#define BUFFER_FRAME 1073741824
#define BUFFER_PAGINA 8388608

typedef struct pagina
{
  unsigned int pfn; //contiene PFN y validez ojo a futuro
  unsigned int validez;
} Pagina ;
typedef struct tablapag
{
  Pagina** paginas; //contiene PFN y validez ojo a futuro

} Tablapag ;

typedef struct crms_file
{
  char validez;
  char* nombre;
  unsigned int tamano;
  char* vpn_offset; //ya no se usa
  unsigned int vpn;
  unsigned int offset;
  struct pcb* process;

} CrmsFile ; //process_id o puntero al proceso.

//entrada!
typedef struct pcb
{
  char estado;
  int id;
  char* nombre;
  CrmsFile** subentradas;
  Tablapag tablapag;  
} Pcb ;

typedef struct bitmap
{
 int* arreglo; 
} Bitmap ;



typedef struct crms
{
  Pcb** tabla_pcb;
  Bitmap* bitmap;
} Crms ; //guardar direccion del .bin para no pasarla como argumento a las funciones
void cr_mount (char* filename);
CrmsFile* cr_open(int process_id, char* file_name, char mode);