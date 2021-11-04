#include <stdio.h>
#include <stdbool.h>  //para datos booleanos

/*DEFINICIÓN DE LAS ESTRUCTURAS*/
typedef struct arbol
{
    unsigned long frec;     //frecuencia
    unsigned char dato;    //Dato
    struct arbol *izq;
    struct arbol *der;
} arbol;

typedef struct lista
{
    arbol *ar;
    struct lista *siguiente;
} lista;

/*DEFINICIPON DE VARIABLES GLOBALESs*/
unsigned char *arregloBitsTemp;   //variable para el arreglo de bits
char bits[256];   //variable para el arreglo de bits
char salida[256];   //variable para el arreglo de bits
unsigned char caracter;       //variable para cada caracter a codificar en el árbol
int nivelFinal; //nivel final de cada caracter en el arbol
int altura = 0; //variable para la altura del arbol
unsigned char codigosHojas[256][256] = {0}; //variable para el código de cada caracter
char nivelHojas[256] = {0};
unsigned long frecuenciasTotal = 0;                 //acumula el total de las frecuencias de los caracteres
/*DEFINICIÓN DE LAS FUNCIONES*/
unsigned long detallesArchivo(FILE *cod);
void agregarLista(lista **l, arbol *a);
void mergeSort(lista **l);
lista *mezcla(lista *a, lista *b);
void sublistas(lista *l, lista **delante, lista **atras);
lista *crearArbol(lista *l);
arbol *unirArboles(arbol *aMayor, arbol *aMenor);
int codificar(arbol *nodo, int nivel);
void imprimirLista(lista *l);
void escribirArchivoFrecuencias(lista *l);
void imprimirArbol(arbol *a);
int alturaArbol(arbol *a);
void decodificarArchivo(unsigned char *datos, unsigned long tam, arbol *raiz, char *nombre);
int codificarHojas(arbol *nodo, int nivel, unsigned char *arregloBitsTemp);