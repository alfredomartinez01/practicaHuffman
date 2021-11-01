#include <stdio.h>
#include <stdbool.h>  //para datos booleanos

/*DEFINICIÓN DE LAS ESTRUCTURAS*/
typedef struct arbol
{
    int frec;     //frecuencia
    char dato;    //Dato
    struct arbol *izq;
    struct arbol *der;
    bool visitado; //en false por que aun no se visita
} arbol;

typedef struct lista
{
    arbol *ar;
    struct lista *siguiente;
} lista;

/*DEFINICIPON DE VARIABLES*/
unsigned char *arregloSalida; //variable para el arreglo de salida
unsigned char *arregloBits;   //variable para el arreglo de bits
char bits[256];   //variable para el arreglo de bits
char salida[256];   //variable para el arreglo de bits
char caracter;       //variable para cada caracter a codificar en el árbol
int nivelFinal; //nivel final de cada caracter en el arbol

/*DEFINICIÓN DE LAS FUNCIONES*/
unsigned long detallesArchivo(FILE *cod);
void agregarLista(lista **l, arbol *a);
lista *crearArbol(lista *l);
arbol *unirArboles(arbol *aMayor, arbol *aMenor);
int codificar(arbol *nodo, int nivel);
void imprimirLista(lista *l);
void escribirArchivoFrecuencias(lista *l);
void imprimirArbol(arbol *a);
int alturaArbol(arbol *a);
