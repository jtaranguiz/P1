#pragma once
#define BUFFER_TABLA 4096
#define BUFFER_BITMAP 16
#define BUFFER_FRAME 1073741824

typedef struct tablapag
{
  char* entradas; //contiene PFN y validez ojo a futuro
} Tablapag ;

typedef struct subentrada
{
  char validez;
  char* nombre;
  int tamaño;
  char vpn;
  int offset;

} Subentrada ;

//entrada!
typedef struct pcb
{
  char estado;
  int id;
  char* nombre;
  Subentrada* subentradas;
  Tablapag* tablapag;  
} Pcb ;

typedef struct bitmap
{
 double mitad1; // mitad de bits de los 128 totales 
 double mitad2; 
} Bitmap ;

typedef struct frame
{
  char contenido; 
} Frame ;

typedef struct crms
{
  Pcb** tabla_pcb;
  Bitmap* bitmap;
  Frame*  frame;
} CrmsFile ;
