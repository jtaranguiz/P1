#pragma once
#define BUFFER_TABLA 4096
#define BUFFER_BITMAP 16
#define BUFFER_FRAME 1073741824
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
  char* nombre ;
  unsigned int tamano;
  char* vpn_offset;
  unsigned int vpn;
  unsigned int offset;

} CrmsFile ; //process_id.

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
} Crms ;
void cr_mount (char* filename);
CrmsFile* cr_open(int process_id, char* file_name, char mode);