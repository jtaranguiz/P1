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
  unsigned int vpn_offset; //ya no se usa
  unsigned int vpn;
  unsigned int offset;
  struct pcb* process;

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
} Crms ; //guardar direccion del .bin para no pasarla como argumento a las funciones
void cr_mount (char* filename);
Crms* asignar(char* filename);
void cr_ls_processes();
int cr_exists(unsigned int process_id, char* filename);
void cr_ls_files(int process_id);
void cr_start_process(int process_id, char* process_name);
void cr_finish_process(int process_id);
unsigned int* find_first_empty(Pcb* process,unsigned int first_vpn,unsigned int first_offset);
unsigned int* find_final_empty(Pcb* process,unsigned int first_vpn,unsigned int first_offset, unsigned int second_vpn, unsigned int second_offset);
int find_tamano_libre(unsigned int first_vpn, unsigned int first_offset, unsigned int second_vpn, unsigned int second_offset);
int cr_write_file(CrmsFile* file_desc, char* buffer, int n_bytes);
CrmsFile* cr_open(int process_id, char* file_name, char mode);
int cr_read(CrmsFile* file_desc, char* buffer, int n_bytes);
unsigned int obtener_dir(unsigned int vpn, unsigned int offset);
void cr_close(CrmsFile* file_desc);
void cr_delete_file(CrmsFile* file_desc);
void liberar_frames(unsigned int vpn, Pcb* proceso);
void free_memory();
void close_memory();
