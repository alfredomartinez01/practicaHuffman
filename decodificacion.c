#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>  //para datos booleanos
#include <sys/stat.h> //para saber detalles de archivos
#include "definiciones.h"

int main(int argc, char const *argv[])
{

    lista *lista = NULL; //variable para la lista
    arbol *arbol; //variable para el arbol

    /*LECTURA DE LA TABLA DE FRECUENCIAS*/
    char caracter; //almacena cada caracter leído 
    int frecuencias; //frecuencia de cada uno de los caracteres del archivo
    int caracteresTotales = 0; // acumula el total de caracteres diferentes del archivo original
    int frecuenciasTotal = 0; //acumula el total de las frecuencias de los caracteres
    char nombreArchivo[50]; //almacena el nombre del archivo
    FILE *tablaFrecuencias = NULL; //variable para el archivo
    unsigned long tam; //varibale que almacena el tamaño del archivo
    int i = 0; //variable para loop

    printf("\nArchivo a decodificar: ");
    scanf("%s", nombreArchivo); //leemos el nombre del archivo a decodificar

    tablaFrecuencias = fopen(nombreArchivo, "rt");

    if (tablaFrecuencias == NULL) //comprobamos que exista el archivo
    { //si el archivo es nulo
        perror("\nNombre incorrecto o no existe el archivo");
        exit(1); //salimos del programa
    }

    //obtenemos los detalles del archivo, en este caso el tamaño
    tam = detallesArchivo(tablaFrecuencias);                                        //lo almacenamos en tam
    printf("\nEl tam del archivo de la tabla de frecuencias es de %lu bytes\n", tam); //se imprime el tamaño del archivo en bytes

    //una vez que tengamos el tamaño del archivo podemos pasar los datos del archivo a un arreglo de caracteres
    char *datos = (char *)malloc(sizeof(char) * tam); //establecemos el tamaño del archivo
    //con ayuda de fread pasamos el contenido del archivo al arreglo anterior
    fread(datos, sizeof(char), tam, tablaFrecuencias);

    while(i < tam){
        arbol = (arbol *)malloc(sizeof(arbol));
        arbol->dato = dato[i]; //se iguala al caracter actual
        arbol->frec = dato[i+1] - '0'; //asignamos la frecuencia(conversión de char a int) del caracter

        agregarLista(&l, arbol); //agregamos al inicio de la lista
     
        frecuenciasTotal+= frecuencia; 
        caracteresTotales+=2;
        i+=2; //avanzamos de par en par
    }

    //Para verificar que se impriman bien y ordenados los caracteres
    imprimirLista(l);

    // -------- CREAMOS EL ÁRBOL ----------
    //se juntan los arboles de la lista en uno solo
    l = crearArbol(l);
    //obtenemos la altura del arbol
    altura = alturaArbol(l->ar);
    printf("\nLa altura el arbol es: %d\n", altura);

    //Función para verificar que el arbol se haya unido correctamente
    // imprimirArbol(l->ar);

    // -------- DECODIFICACIÓN ----------
    unsigned char arregloEntrada; //arreglo de todos los bits de entrada
    arregloEntrada = (unsigned char *)malloc(sizeof(unsigned char) * altura); // Inicializamos el arreglo temporal que guardará los bits que será la altura del árbol (para el peor caso)
    int i = 0; //variable para loop

    FILE *decodificar = NULL; //varaible para el archivo a decodificar

    printf("\nArchivo a decodificar: ");
    scanf("%s", nombreArchivo); //leemos el nombre del archivo a decodificar

    decodificar = fopen(nombreArchivo, "rb");

    if (decodificar == NULL) //comprobamos que exista el archivo
    { //si el archivo es nulo
        perror("\nNombre incorrecto o no existe el archivo");
        exit(1); //salimos del programa
    }

    //obtenemos los detalles del archivo, en este caso el tamaño
    tam = detallesArchivo(decodificar);                                        //lo almacenamos en tam
    printf("\nEl tam del archivo a decodificar es de %lu bytes\n", tam); //se imprime el tamaño del archivo en bytes

 

    while(i < tam){

    }
    
}

/*
    Función para obtener los detalles del archivo, en este caso nos interesa el tamaño.
    Se hizo uso de la libreria sys/stat.h.
    REcibe el archivo del que deseamos saber su tamaño
    Retorna el tamaño del archivo

*/
unsigned long detallesArchivo(FILE *cod)
{
    struct stat buffer; //estructura para recuperar atributos del archivo

    unsigned long descriptor = fileno(cod); //regresa el descriptor de archivo
    //con la llamada a la funcion fstat obtenemos detalles del archivo
    if (fstat(descriptor, &buffer) == -1)
    { //si regresa -1 hubo un error
        perror("Error en fstat");
        exit(1); //Salimos del programa
    }

    return buffer.st_size; //Retornamos el tamaño del archivo
}


/*
    Función que agrega un elemento al final de la lista
    Recibe la lista donde se insertará el elemento y el elemento a insertar
    en este caso el arbol con el dato y la frecuencia
    Retorna el nuevo elemento 
*/
void agregarLista(lista **l, arbol *a)
{
    lista *nuevoArbol, *aux;                           //creamos una variable para alamacenar el dato
    nuevoArbol = (lista *)malloc(sizeof(lista)); //le asignamos espacio al dato
    nuevoArbol->ar = a;                          //guardamos el arbol en el nuevo nodo
    nuevoArbol->siguiente = NULL;                  //el siguiente nodo será igual a la lista que teniamos
    
    if(*l == NULL){ //verificamos si la lsita esta vacía
        *l = nuevoArbol; //agregar primer elemento
    } else {
        aux = *l; 
        while(aux->siguiente != NULL){ //cuando ya hay por lo menos un elemento en la lista
            aux = aux->siguiente; //apuntamos al siguiente nodo
        }
        aux->siguiente = nuevoArbol; //agregamos un nuevo nodo al final de la lista
    }
}

/*
    Función auxiliar para imprimir los valores de la lista.
    Recibe la lista que imorimira y no retorna nada ya que solo imprime el contenido
    de la lista
*/
void imprimirLista(lista *l)
{
    int k = 0;
    //Mientras la lista no sea NULL ingresa al while
    while (l != NULL)
    {
        printf("\n%c\t%d", l->ar->dato, l->ar->frec); //imprime el dato del arbol actual
        // printf("\nFrecuencia %d ", l->ar.frec); //imprime la frecuencia del arbol actual
        l = l->siguiente; //avanzamos al siguiente nodo de la lista
    }
}

/*
    Funcion que crea el arbol a partir de ir uniendo dos arboles de la lista 
    y sumando sus frecuencias.
    Recibe la lista que contiene los arboles que se juntarán.
    Regresa la lista con un solo arbol que fue unido
*/
lista *crearArbol(lista *l)
{
    
    while (l->siguiente != NULL) //mientras haya elementos en la lista
    {
        //Establecemos dos listas, una donde se ira almacenando el arbol unido y otra que servira como auxilair
        lista *arbolUnido = l;
        lista *aux = l;
        //Como iremos comparando de dos en dos los arboles, recorremos la lista en dos elementos
        l = l->siguiente->siguiente;

        //mandamos a llamar la función que une los arboles
        arbolUnido->ar = unirArboles(arbolUnido->siguiente->ar, arbolUnido->ar);

        /*
            SI la lista es nula o si la frecuencia del arbol unido anteriormente es menor
            a la frecuencia del arbol, haremos que el nuevo elemento de la lista sea el elemento
            desde el cual se recorrio la lista y la lista será el elemento unido
        */
        if (l == NULL || arbolUnido->ar->frec <= l->ar->frec)
        {
            arbolUnido->siguiente = l; //el siguiente elemento será al que apunto al recorrer
            l = arbolUnido;            //la lista tserá el arbol unido
        }
        /*
            Si la lista no es nula y la frecuencia del arbol unido es mayor a la del arbol
            haremos uso del auxiliar guardando ahi la lista y avanzando hasta que se encuentre
            la posicion correcta del nuevo nodo que es el arbol unido.
        */
        if (l != NULL && arbolUnido->ar->frec > l->ar->frec)
        {
            aux = l;
            //Recorremos el auxiliar hasta que sea nulo o se encuentre un elemento mayor al arbol unido
            while (aux->siguiente != NULL && arbolUnido->ar->frec > aux->ar->frec)
            {
                aux = aux->siguiente; //avanzamos en el auxiliar
            }
            arbolUnido->siguiente = aux->siguiente; //se le asigna la posicion donde se guardara el nuevo arbol
            aux->siguiente = arbolUnido;            //almacenamos el nuevo arbol en la posicion
        }
    }

    return l; //retornamos la lista unida
}

/*
    Esta función une los arboles, para hacer esto se crea un nuevo arbol y se almacena en el
    la frecuencia del arbol mayor y menor para asi formar un solo arbol
    Recibe el arbol con el caracter con mayor frecuencia y un arbol con el caracter de menor frecuencia
    Retorna el nuevo arbol que se almacenara en la lista
*/
arbol *unirArboles(arbol *aMayor, arbol *aMenor)
{
    arbol *nuevo = (arbol *)malloc(sizeof(arbol)); //Creamos el nuevo arbol

    nuevo->frec = aMayor->frec + aMenor->frec; //Sumamos las frecuencias del arbol
    nuevo->izq = aMenor;                       //se le asigna a la izquierda el arbol con menor frecuencia
    nuevo->der = aMayor;                       //se le asigna a la derecha el arbol con mayor frecuencia

    return nuevo; //Retornamos el nuevo arbol
}

/*
    Función auxiliar para imprimir el arbol
    Recibe el arbol a imprimir
    No devuelve nada
*/
void imprimirArbol(arbol *a)
{
    //Mientras el nodo no sea NULL se imprime el valor
    if (a != NULL)
    {
        imprimirArbol(a->izq); //imprimimos por la izquierda
        printf("\n%d", a->frec);
        imprimirArbol(a->der); //imprmimimos por la derecha
    }
}

/*
    FUnción para calcular la altura del arbol.
    Recibe la altura del arbol del que se desea saber la altura
    Devuelve la ltura del arbol
*/
int alturaArbol(arbol *a)
{
    //Si no hay elemntos la altura sera 0
    if (!a)
    {
        return 0;
    }
    int alt = 0; //para almacenar la altura

    //calculamos la altura de la derecha
    int alturaDerecha = alturaArbol(a->der);
    //calculamos la altura de la izquierda
    int alturaIzquierda = alturaArbol(a->izq);

    //la altura mayor se guardará como la altura del arbol
    alt = alturaIzquierda > alturaDerecha ? alturaIzquierda + 1 : alturaDerecha + 1;

    return alt; //Retornamos la altura
}
