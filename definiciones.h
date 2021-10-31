
/*DEFINICIÓN DE LAS ESTRUCTURAS*/
typedef struct arbol{
    int etiqueta; //la etiqueta será 0 o 1
    int frec; //frecuencia
    char dato; //Dato
    struct arbol *izq;
    struct arbol *der;
}arbol;

typedef struct lista{
    arbol *ar;
    struct lista *siguiente;
}lista;

/*DEFINICIÓN DE LAS FUNCIONES*/
unsigned long detallesArchivo(FILE *cod);
void agregarLista(lista **l, arbol *a);
void mergeSort(lista **l);
lista *mezcla(lista *a, lista *b);
void sublistas(lista *l, lista **delante, lista**atras);
lista* crearArbol(lista *l);
arbol *unirArboles(arbol *aMayor, arbol *aMenor);
