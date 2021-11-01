#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> //para datos booleanos
#include <sys/stat.h> //para saber detalles de archivos
#include "definiciones.h"

int main(int argc, char const *argv[])
{
    unsigned long tam; //varibale que almacena el tamaño del archivo
    char archivo[50]; //variable que almacena el nombre del archivo a leer
    FILE *frecuencias = NULL, *cod = NULL; //Variables para los archivos
    int i = 0; //varibale para loops
    lista *l = NULL; //variable para la lista
    bool encontrado; //variable que servirá para saber si un elemento se encuentra en el arbol
    arbol *a; //variable para el arbol

    printf("\nArchivo a codificar: ");
    scanf("%s", archivo); //leemos el nombre del archivo a codificar

    cod = fopen(archivo,"rb"); //abrimos el archivo en modo lectura binaria
    
    if(cod == NULL){ //si el archivo es nulo
        perror("\nNombre incorrecto o no existe el archivo");
        exit(1); //salimos del programa
    }

    //obtenemos los detalles del archivo, en este caso el tamaño
    tam = detallesArchivo(cod); //lo almacenamos en tam
    printf("\nEl tamaño del archivo a codificar es de %lu bytes\n", tam);//se imprime el tamaño del archivo en bytes

    //una vez que tengamos el tamaño del archivo podemos pasar los datos del archivo a un arreglo de caracteres
    char *datos = (char *)malloc(sizeof(char)*tam); //establecemos el tamaño del archivo
    //con ayuda de fread pasamos el contenido del archivo al arreglo anterior
    fread(datos, sizeof(char),tam,cod);
    // for(i=0;i<tam;i++)
    //     printf("\n%c\n",datos[i]); //para verificar datos del archivo

    while(i < tam){
        encontrado = false; //al inicio sera false (se uso stdbool.h)
        lista *lAux = l;//lista auxiliar para recorrerla
        //Recorremos la lista
        while( lAux != NULL ){
            //si el caracter de los datos ya esta en un arbol se incrementa su frecuencia
            if(datos[i] == lAux->ar->dato){
                lAux->ar->frec++;//incrementamos la frecuencia del caracter
                encontrado = true; //ponemos en true ya que se ha encontrado

                // if(lAux->siguiente != NULL && lAux->siguiente->ar.frec < lAux->ar.frec){
                //     arbol aux;
                //     aux = lAux->ar;
                //     lAux->ar = lAux->siguiente->ar;
                //     lAux->siguiente->ar = aux;
                // }
            }

            lAux = lAux->siguiente; //Avanzamos al siguiente nodo de la lista
        }

        //Si no esta el elemento lo agregamos al arbol y su frecuencia será de 0
        if(!encontrado){
            a = (arbol *)malloc(sizeof(arbol));
            a->dato = datos[i]; //se iguala al caracter actual
            a->frec = 1; //frecuencia en 0

            agregarLista(&l, a); //agregamos al inicio de la lista
        }
        i++; //incrementamos i en uno
    }
    //Para verificar que se impriman bien los caracteres
    imprimirLista(l);

    //ordenamos la lista y los arboles dentro de ella de menor a mayor frecuencia usando merge
    mergeSort(&l);

    //Imrpimimos valores de la lista solo para corroborar que esten bien las frecuencias
    // printf("\n LIsta ordenada\n");
    // imprimirLista(l);

    escribirArchivoFrecuencias(l);

    //se juntan los arboles de la lista en uno solo
    l = crearArbol(l);

    printf("\nLista ordenada\n");
    imprimirLista(l);
    
    printf("\n\n");
    return 0;
}

/*
    Función para obtener los detalles del archivo, en este caso nos interesa el tamaño.
    Se hizo uso de la libreria sys/stat.h.
    REcibe el archivo del que deseamos saber su tamaño
    Retorna el tamaño del archivo

*/
unsigned long detallesArchivo(FILE *cod){
    struct stat buffer; //estructura para recuperar atributos del archivo

    unsigned long descriptor = fileno(cod); //regresa el descriptor de archivo 
    //con la llamada a la funcion fstat obtenemos detalles del archivo
    if(fstat(descriptor, &buffer) == -1){ //si regresa -1 hubo un error
        perror("Error en fstat");
        exit(1); //Salimos del programa
    }

    return buffer.st_size; //Retornamos el tamaño del archivo
}


/*
    Función que agrega un elemento al inicio de la lista
    Recibe la lista donde se insertará el elemento y el elemento a insertar
    en este caso el arbol con el dato y la frecuencia
    Retorna el nuevo elemento 
*/
void agregarLista(lista **l, arbol *a){
    lista *nuevoArbol; //creamos una variable para alamacenar el dato
    nuevoArbol = (lista *)malloc(sizeof(lista)); //le asignamos espacio al dato
    nuevoArbol->ar = a; //guardamos el arbol en el nuevo nodo
    nuevoArbol->siguiente = *l; //el siguiente nodo será igual a la lista que teniamos
    (*l) = nuevoArbol; 
   
    
}

/*
    Función auxiliar para imprimir los valores de la lista.
    Recibe la lista que imorimira y no retorna nada ya que solo imprime el contenido
    de la lista
*/
void imprimirLista(lista *l){
    int k = 0;
    //Mientras la lista no sea NULL ingresa al while
     while(l != NULL){
        printf("\n%c\t%d", l->ar->dato,l->ar->frec); //imprime el dato del arbol actual
        // printf("\nFrecuencia %d ", l->ar.frec); //imprime la frecuencia del arbol actual
        l = l->siguiente; //avanzamos al siguiente nodo de la lista
    }
}

/*
    La siguiente función es para ordenar nuestra lista, se uso mergeSort resultando en una complejidad O(nLogn)
    Recibe la lista que será ordenada bajo este método
    No regresa nada ya que se esta modificando directamente la lista
*/
void mergeSort(lista **l){
    lista *inicio = *l; //establecemos la cabeza de la lista
    lista *a, *b; //Establecemos los nodos

    //Si la lista es nula o solo hay un arbol no ordenamos
    if(( inicio == NULL ) || ( inicio->siguiente == NULL ))
        return; //salimos de la funcion

    //Función para hacer sublistas de la lista
    sublistas(inicio, &a, &b);

    //Llamada recursiva a merge para las sublistas creadas
    mergeSort(&a); //merge para la sublista a
    mergeSort(&b); //merge para la sublista b

    //Juntamos las dos sublistas ordenadas
    *l = mezcla(a, b);
}

/*
    Función que realiza el merge para tener la lista completamente ordenada
    Recibe la sublista a y sublista b a mezclar
    Regresa la lista mezclada
*/
lista *mezcla(lista *a, lista *b){
    lista *resultado = NULL; //variable que almacenara la lista completa

    //Si a es nula regresamos la lista b solamente y viceversa, serán los casos base
    if(a == NULL)
        return b; //regresamos la sublista b
    else if(b == NULL)
        return a; //regresamos la sublista a

    /*
        Comparamos las frecuencias de los arboles dentro de las dos sublistas
        Si la frecuencia del arbol de la sublista a es menor o igual que el del arbol de la sublista b
        se le asigna a al resultado, posteriomente se avanza un nodo y se llama recursivamente 
        a la función para hacer la misma comparación, pero con el siguiente nodo
        Si la comparación no se cumple entonces el resultado se asigna a b y se repite lo anterior.
    */
    if(a->ar->frec <= b->ar->frec){
        resultado = a; 
        resultado->siguiente = mezcla(a->siguiente, b); //Llamada recursiva a a la mezcla
    }else{
        resultado = b;
        resultado->siguiente = mezcla(a, b->siguiente); //llamada recursiva a la mezcla
    }

    return resultado; //retornmaos el resultaado
}

/*
    Esta función parte la lista en dos sublistas para comenzar el ordenamiento
    Recibe la lista a partir, la sublista que sera la aprte de atras y la que sera la parte de adelante
    No retorna nada, pues se modifican los nodos por referencia.

*/
void sublistas(lista *l, lista **delante, lista **atras){
    lista *avanzaDosNodos = l->siguiente; //variable con la que avanzaremos dos nodos de la lista
    lista *avanzaUnNodo = l; //variable para avanzar un nodo de la lista

    //Mientras se pueda avanzar en la lista avanzamos dos nodos
    while(avanzaDosNodos != NULL){
        avanzaDosNodos = avanzaDosNodos->siguiente; //avnazamos dos nodos de la lista
        if(avanzaDosNodos != NULL){ //SI aun no llega al final procedemos a avanzar un nodo
            avanzaUnNodo = avanzaUnNodo->siguiente; //avanzamos un nodo
            avanzaDosNodos = avanzaDosNodos->siguiente; //Seguimos avanzando dos nodos
        }
    }

    
    *delante = l; //almacenamos la parte de adelante
     //la parte de atras se asigna a avanzaUnNodo ya que esta queda antes de la mitad de la lista
    *atras = avanzaUnNodo->siguiente;
    avanzaUnNodo->siguiente = NULL; //el siguiente nodo en NULL
}

/*
    Función que escribe el archivo de frecuencias con los elementros dentro de la lista
    Recibe la lista que contiene los elementos a escribir
    No retorna nada, pero creo un archivo frecuencias.txt con los caracteres y su frecuencia
*/
void escribirArchivoFrecuencias(lista *l){
    FILE *frecuencias; //variable para el archivo

    frecuencias = fopen("frecuencias.txt", "wt"); //se abre o crea el archivo en modo escritura

    //Recorremos la lista para impriomir en el archivo 
    while(l != NULL){
        //colocamos el caracter junto con su frecuencia
        fprintf(frecuencias, "%c%d", l->ar->dato, l->ar->frec);
        l = l->siguiente; //avanzamos al siguiente nodo
    }

    fclose(frecuencias); //Cerramos el archivo
}

/*
    Funcion que crea el arbol a partir de ir uniendo dos arboles de la lista 
    y sumando sus frecuencias.
    Recibe la lista que contiene los arboles que se juntarán.
    Regresa la lista con un solo arbol que fue unido
*/
lista* crearArbol(lista *l){
    //vamos recorriendo  la lista mientras el siguiente nodo no sea nulo
    while(l->siguiente != NULL){
        //Establecemos dos listas, una donde se ira almacenando el arbol unido y otra que servira como auxilair 
        lista *arbolUnido = l;
        lista *aux = l;
        //Como iremos comparando de dos en dos los arboles, recorremos la lista en dos elementos
        l = l->siguiente->siguiente;

        //mandamos a llamar la función que une los arboles
        arbolUnido->ar = unirArboles(arbolUnido->siguiente->ar,arbolUnido->ar);
        
        /*
            SI la lista es nula o si la frecuencia del arbol unido anteriormente es menor
            a la frecuencia del arbol, haremos que el nuevo elemento de la lista sea el elemento
            desde el cual se recorrio la lista y la lista será el elemento unido
        */
        if(l == NULL || arbolUnido->ar->frec <= l->ar->frec ){
            arbolUnido->siguiente = l; //el siguiente elemento será al que apunto al recorrer
            l = arbolUnido; //la lista tserá el arbol unido
        }
        /*
            Si la lista no es nula y la frecuencia del arbol unido es mayor a la del arbol
            haremos uso del auxiliar guardando ahi la lista y avanzando hasta que se encuentre
            la posicion correcta del nuevo nodo que es el arbol unido.
        */
        if(l != NULL && arbolUnido->ar->frec > l->ar->frec){
            aux = l;
            //Recorremos el auxiliar hasta que sea nulo o se encuentre un elemento mayor al arbol unido
            while(aux->siguiente != NULL && arbolUnido->ar->frec > aux->ar->frec){
                aux = aux->siguiente; //avanzamos en el auxiliar
            }
            arbolUnido->siguiente = aux->siguiente; //se le asigna la posicion donde se guardara el nuevo arbol
            aux->siguiente = arbolUnido; //almacenamos el nuevo arbol en la posicion
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
arbol *unirArboles(arbol *aMayor, arbol *aMenor){
    arbol *nuevo = (arbol *)malloc(sizeof(arbol)); //Creamos el nuevo arbol

    nuevo->frec = aMayor->frec + aMenor->frec; //Sumamos las frecuencias del arbol
    nuevo->izq = aMenor; //se le asigna a la izquierda el arbol con menor frecuencia
    nuevo->der = aMayor; //se le asigna a la derecha el arbol con mayor frecuencia
    nuevo->izq->etiqueta = 0; //a la izquierda lo etiquetamos con un 0
    nuevo->der->etiqueta = 1; //a la derecha lo etiquetamos con un 1

    return nuevo; //Retornamos el nuevo arbol
}

