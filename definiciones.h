#include <stdio.h>

/*DEFINICIÓN DE LAS ESTRUCTURAS*/
typedef struct arbol
{
    int frec;     //frecuencia
    char dato;    //Dato
    struct arbol *izq;
    struct arbol *der;
} arbol;

typedef struct lista
{
    arbol *ar;
    struct lista *siguiente;
} lista;

/*DEFINICIPON DE VARIABLES*/
unsigned char *arregloSalida; //variable para el arreglo de salida
unsigned char *arregloBits;   //variable para el arreglo de bits
char caracter;       //variable para cada caracter a codificar en el árbol
int nivelFinal; //nivel final de cada caracter en el arbol

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
void guardarCodificacion(char *arregloSalida, char *archivo, int tam);