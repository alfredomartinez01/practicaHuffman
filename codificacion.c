#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>  //para datos booleanos
#include <sys/stat.h> //para saber detalles de archivos
#include "definiciones.h"

int altura = 0; //variable para la altura del arbol

int main(int argc, char const *argv[])
{
    unsigned long tam;                     //varibale que almacena el tamaño del archivo
    char archivo[50];                      //variable que almacena el nombre del archivo a leer
    FILE *frecuencias = NULL, *cod = NULL; //Variables para los archivos
    int i = 0;                             //varibale para loops
    lista *l = NULL;                       //variable para la lista
    bool encontrado;                       //variable que servirá para saber si un elemento se encuentra en el arbol
    arbol *a;                              //variable para el arbol
    int bit = 7;                           //para hacer el corrimiento de bits
    char auxCara = 0;                      //Auxiliar que nos servirá para almacenar los caracteres codificados

    printf("\nArchivo a codificar: ");
    scanf("%s", archivo); //leemos el nombre del archivo a codificar

    cod = fopen(archivo, "rb"); //abrimos el archivo en modo lectura binaria

    if (cod == NULL)
    { //si el archivo es nulo
        perror("\nNombre incorrecto o no existe el archivo");
        exit(1); //salimos del programa
    }

    //obtenemos los detalles del archivo, en este caso el tamaño
    tam = detallesArchivo(cod);                                        //lo almacenamos en tam
    printf("\nEl tam del archivo a codificar es de %lu bytes\n", tam); //se imprime el tamaño del archivo en bytes

    //una vez que tengamos el tamaño del archivo podemos pasar los datos del archivo a un arreglo de caracteres
    char *datos = (char *)malloc(sizeof(char) * tam); //establecemos el tamaño del archivo
    //con ayuda de fread pasamos el contenido del archivo al arreglo anterior
    fread(datos, sizeof(char), tam, cod);
    // for(i=0;i<tam;i++)
    //     printf("\n%c\n",datos[i]); //para verificar datos del archivo

    while (i < tam)
    {
        encontrado = false; //al inicio sera false (se uso stdbool.h)
        lista *lAux = l;    //lista auxiliar para recorrerla
        //Recorremos la lista
        while (lAux != NULL)
        {
            //si el caracter de los datos ya esta en un arbol se incrementa su frecuencia
            if (datos[i] == lAux->ar->dato)
            {
                lAux->ar->frec++;  //incrementamos la frecuencia del caracter
                encontrado = true; //ponemos en true ya que se ha encontrado

                // Recorremos el resto de la lista para acomodar en la posición que le corresponde respecto a su frecuencia
                while (lAux->siguiente != NULL && lAux->ar->frec > lAux->siguiente->ar->frec)
                {
                    // Vamos intercambiando nodos
                    arbol *aux;
                    aux = lAux->ar;
                    lAux->ar = lAux->siguiente->ar;
                    lAux->siguiente->ar = aux;

                    // Vamos a la siguiente posición
                    lAux = lAux->siguiente;
                }
                break;
            }

            lAux = lAux->siguiente; //Avanzamos al siguiente nodo de la lista
        }

        //Si no esta el elemento lo agregamos al arbol y su frecuencia será de 0
        if (!encontrado)
        {
            a = (arbol *)malloc(sizeof(arbol));
            a->dato = datos[i];  //se iguala al caracter actual
            a->frec = 1;         //frecuencia en 0
            a->visitado = false; //no se ha visitado

            agregarLista(&l, a); //agregamos al inicio de la lista
        }
        i++; //incrementamos i en uno
    }
    //Para verificar que se impriman bien y ordenados los caracteres
    imprimirLista(l);

    escribirArchivoFrecuencias(l);

    //se juntan los arboles de la lista en uno solo
    l = crearArbol(l);

    //Función para verificar que el arbol se haya unido correctamente
    // imprimirArbol(l->ar);

    altura = alturaArbol(l->ar);

    printf("\nLa altura el arbol es: %d\n", altura);

    // Comenzando la etapa de codificación
    int indice = 0;                                                                // Inicializamos el índice para el arreglo de salida
    //arregloSalida = (unsigned char *)malloc(sizeof(unsigned char) * tam * altura); // Inicializamos el arreglo de salida que será el número de datos por el alto del árbol (para el peor caso)
    arregloBits = (unsigned char *)malloc(sizeof(unsigned char) * altura);         // Inicializamos el arreglo de salida que será la altura del árbol (para el peor caso)
    int j, m = 0;

    FILE *codificado = NULL; //variable para el archivo codificado

    strcat(archivo, ".dat"); //concatenamos .dat para la salida del archivo

    codificado = fopen(archivo, "wb");

    // Recorremos cada uno de los caracteres a codificar y una vez codificados lo escribirmos en el archivo
    for (i = 0; i < tam; i++)
    {
        caracter = datos[i]; // Almacenamos temporalmente cada caracter
        nivelFinal = 0;      // Inicializamos en cero el nivel final

        codificar(l->ar, 0); // Codificamos cada caracter sobre el arregloBits

        // Pasamos todos los bits del arreglo temporal (arregloBits) al archivo, verificando cuando se complete cada byte
        for (j = indice; j < indice + nivelFinal; j++)
        {
            //si ya se hicieron todos los corrimientos
            if (bit < 0)
            {
                bit = 7;                            //comenzamos de nuevo los bits para los corrimientos
                fprintf(codificado, "%c", auxCara); //imprimimos el caracter codificado en el archivo final
                auxCara = 0;                        //lo volvemos a empezar para el nuevo caracter
            }

            //Hacemos la compresion haciendo corrimientos ayudandonos de la variable bit
            //Así llevamos la cuenta de los bits procesados, cuando se procesan los 8 bits escribimos en el archivo
            auxCara = auxCara | (arregloBits[j - indice] << bit);
            // printf("auxCara: %c\n", auxCara);
            bit--; //vamos retrocediendo el bit para el corrimiento
        }

        indice = (indice + nivelFinal); // Aumentamos el indice correspondiente a la cantidad de bits que se almacenaron en el arreglo de salida
    }

    // En caso de que hayan quedado un byte incompleto, lo escribimos en el archivo
    if (bit >= 0)
    {
        fprintf(codificado, "%c", auxCara); //imprimimos el caracter codificado en el archivo final
    }

    fclose(codificado);
    printf("\nTamaño final de: %d\n", indice);
    printf("\n\n");
    return 0; //salimos
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
    Función que agrega un elemento al inicio de la lista
    Recibe la lista donde se insertará el elemento y el elemento a insertar
    en este caso el arbol con el dato y la frecuencia
    Retorna el nuevo elemento 
*/
void agregarLista(lista **l, arbol *a)
{
    lista *nuevoArbol;                           //creamos una variable para alamacenar el dato
    nuevoArbol = (lista *)malloc(sizeof(lista)); //le asignamos espacio al dato
    nuevoArbol->ar = a;                          //guardamos el arbol en el nuevo nodo
    nuevoArbol->siguiente = *l;                  //el siguiente nodo será igual a la lista que teniamos
    (*l) = nuevoArbol;
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
    Función que escribe el archivo de frecuencias con los elementros dentro de la lista
    Recibe la lista que contiene los elementos a escribir
    No retorna nada, pero creo un archivo frecuencias.txt con los caracteres y su frecuencia
*/
void escribirArchivoFrecuencias(lista *l)
{
    FILE *frecuencias; //variable para el archivo

    frecuencias = fopen("frecuencias.txt", "wt"); //se abre o crea el archivo en modo escritura

    //Recorremos la lista para impriomir en el archivo
    while (l != NULL)
    {
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
lista *crearArbol(lista *l)
{
    //vamos recorriendo  la lista mientras el siguiente nodo no sea nulo
    while (l->siguiente != NULL)
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
    Función que codifica cada caracter para almacenar en arregloBits los bits correspondientes al camino sobre el árbol
    Recibe el nodo del arbol y el nivel en que se encuentra sobre el mismo
    Devuelve 1 si encontró en si mismo o alguna de sus nodos hijos el dato, devuelve 0 en caso contrario
*/
int codificar(arbol *nodo, int nivel)
{
    // Si el nodo actual no está vacío
    if (nodo != NULL)
    {
        // Comprobamos que el nodo no haya sido visitado
        if (!nodo->visitado)
        {

            // En caso de que ya haya encontrado la hoja que contiene al caracter
            if (nodo->dato == caracter)
            {
                nivelFinal = nivel;
                //nodo->visitado = true;
                // printf("dato:%c, nivel: %d\n",nodo->dato, nivelFinal);
                return 1;
            }
            // Recorremos el nodo derecho para buscarlo
            if (codificar(nodo->der, nivel + 1))
            {
                arregloBits[nivel] = 1;
                // printf("Arreglo bits: %u\n", arregloBits[nivel]);
                return 1;
            }
            // Recorremos el nodo izquierdo para buscarlos
            if (codificar(nodo->izq, nivel + 1))
            {
                arregloBits[nivel] = 0;
                //printf("Arreglo bits: %u\n", arregloBits[nivel]);
                return 1;
            }
        }
    }

    // En caso de que sea NULL o no se haya encontrado
    return 0;
}

/*
    Función auxiliar para imprimir el arbol
    Recibe el arbol a imprimir
    No devuelve nada
*/
void imprimirArbol(arbol *a)
{

    //MIentras el nodo no sea NULL se imprime el valor
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
