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

} CrmsFile ;

//entrada!
typedef struct pcb
{
  char estado;
  unsigned int id;
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
Crms* asignar(char* filename);
void cr_ls_processes();
int cr_exists(unsigned int process_id, char* filename);
void cr_ls_files(int process_id);
void cr_start_process(int process_id, char* process_name);
void cr_finish_process(int process_id);