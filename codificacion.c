#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> //para datos booleanos
#include <sys/stat.h> //para saber detalles de archivos


typedef struct arbol{
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
lista* agregarLista(lista *l, arbol a);

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

lista* agregarLista(lista *l, arbol a){
    lista *nuevoArbol;
    nuevoArbol = (lista *)malloc(sizeof(lista));
    nuevoArbol->ar = a;
    nuevoArbol->siguiente = l;
    l = nuevoArbol;

    return nuevoArbol;
    
}



int main(int argc, char const *argv[])
{
    unsigned long tam; //vairbale que almacena el tamaño del archivo
    char archivo[50]; //variable que almacena el nombre del archivo a leer
    FILE *frecuencias = NULL, *cod = NULL; //Variables para los archivos
    int i; //varibale para loops
    lista *l; //variable para la lista
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
    for(i=0;i<tam;i++)
        printf("\n%c\n",datos[i]); //para verificar datos del archivo
    
    // arbol arboles[tam];

    // for(i=0;i<tam;i++){
    //     arboles[i].frec = 0;
    //     arboles[i].dato = datos[i];
    //     arboles[i].der = arboles[i].izq = NULL;
    // }
    // printf("\n%c\n",arboles[0].dato); //para verificar datos del archivo

    l = NULL; //ponemos en NULL la lista
    i = 0;
    while(i < tam){
        encontrado = false;
        // lista *auxL = l; //lista auxiliar para recorrerla
        lista *lAux = l;
        while( lAux != NULL ){
            //si el caracter de los datos ya esta en un arbol se incrementa su frecuencia
            if(datos[i] == lAux->ar.dato){
                lAux->ar.frec++;//incrementamos la frecuencia del caracter
                encontrado = true; //ponemos en true ya que se ha encontrado

                // if(lAux->siguiente != NULL && lAux->siguiente->ar.frec < lAux->ar.frec){
                //     arbol aux;;
                //     aux = lAux->ar;
                //     lAux->ar = lAux->siguiente->ar;
                //     lAux->siguiente->ar = aux;
                // }
            }

            lAux = lAux->siguiente; 
        }

        //Si no esta el elemento lo agregamos al arbol y su frecuencia será de 0
        if(!encontrado){
            a.dato = datos[i]; //se iguala al caracter actual
            a.frec = 1; //frecuencia en 0
            // printf("\nElemento %c ", a.dato);

            l = agregarLista(l, a); //agregamos al inicio de la lista
            // i = 0; //para que haga de nuevo la busqueda
        }
        i++;

     
    }
       while(l != NULL){
        printf("\nElemento %c ", l->ar.dato);
        printf("\nFrecuencia %d ", l->ar.frec);
        l = l->siguiente;
        }

    printf("\n");

 


    return 0;
}
