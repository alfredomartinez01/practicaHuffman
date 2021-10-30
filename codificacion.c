#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> //para datos booleanos
#include <sys/stat.h> //para saber detalles de archivos


typedef struct arbol{
    int etiqueta; //la etiqueta será 0 o 1
    int frec; //frecuencia
    char dato; //Dato
    struct arbol *izq;
    struct arbol *der;
}arbol;

typedef struct lista{
    arbol ar;
    struct lista *siguiente;
}lista;

unsigned long detallesArchivo(FILE *cod);
void agregarLista(lista **l, arbol a);
void mergeSort(lista **l);
lista *mezcla(lista *a, lista *b);
void sublistas(lista *l, lista **delante, lista**atras);

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
void agregarLista(lista **l, arbol a){
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
        printf("\n%d\t%c\t%d ",++k, l->ar.dato,l->ar.frec); //imprime el dato del arbol actual
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
    if(a->ar.frec <= b->ar.frec){
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



int main(int argc, char const *argv[])
{
    unsigned long tam; //vairbale que almacena el tamaño del archivo
    char archivo[50]; //variable que almacena el nombre del archivo a leer
    FILE *frecuencias = NULL, *cod = NULL; //Variables para los archivos
    int i = 0; //varibale para loops
    lista *l = NULL; //variable para la lista
    bool encontrado; //variable que servirá para saber si un elemento se encuentra en el arbol
    arbol a; //variable para el arbol

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
            if(datos[i] == lAux->ar.dato){
                lAux->ar.frec++;//incrementamos la frecuencia del caracter
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
            a.dato = datos[i]; //se iguala al caracter actual
            a.frec = 1; //frecuencia en 0

            agregarLista(&l, a); //agregamos al inicio de la lista
        }
        i++; //incrementamos i en uno
    }
    //Para verificar que se impriman bien los caracteres
    imprimirLista(l);

    //ordenamos la lista y los arboles dentro de ella de menor a mayor frecuencia usando merge
    mergeSort(&l);

    //Imrpimimos valores de la lista solo para corroborar que esten bien las frecuencias
    printf("\n LIsta ordenada\n");
    imprimirLista(l);

    
    
    
    printf("\n\n");
    return 0;
}
