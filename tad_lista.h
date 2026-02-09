/* --------------------------------------------------------
   Esta parte define la estructura de lista enlazada y las 
   operaciones necesarias para recorrer, buscar y ordenar 
   tareas. La lista se usa como estructura auxiliar para 
   búsquedas por etiqueta y ordenamiento por fecha.
-------------------------------------------------------- */

#ifndef TAD_LISTA_H   // protege al archivo de ser incluido más de una vez
#define TAD_LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "tad_tarea.h"  // importa tDatosTarea para almacenar cada tarea en los nodos
#include "tad_cola.h"   // para cargar tareas desde las colas

// constante:

#define MAX_ETIQUETA 30

// tipo de dato personalizado: 

typedef char tStringEtiqueta[MAX_ETIQUETA];

// estructura de la lista:

typedef struct nodoLista {
    tDatosTarea tarea;
    struct nodoLista * siguiente;
} tNodoListaTarea;

typedef struct {
    tNodoListaTarea * inicio;
} tListaTareas;

// Prototipos 

void inicializarListaTareas(tListaTareas * lista);
bool listaTareasVacia(tListaTareas lista);
void agregarTareaLista(tListaTareas * lista, tDatosTarea tarea);
void mostrarListaTareas(tListaTareas lista);
void cargarListaDesdeColas(tListaTareas * lista, tCola * alta, tCola * media, tCola * baja);
void buscarTareasPorEtiqueta(tListaTareas lista, tStringEtiqueta etiqueta);
void ordenarListaPorFecha(tListaTareas * lista);
void vaciarListaTareas(tListaTareas * lista);

/* Funciones internas del merge sort, el tipo de ordenamiento que decidimos utilizar,  
porque es eficiente para listas enlazadas y no requiere índices.
Se divide la lista en dos mitades, se ordenan por separado y luego se unen nuevamente en orden. */

tNodoListaTarea * mergeSortLista(tNodoListaTarea * inicio);
void dividirLista(tNodoListaTarea * origen, tNodoListaTarea ** mitad1, tNodoListaTarea ** mitad2);
tNodoListaTarea * mezclarListasPorFecha(tNodoListaTarea * a, tNodoListaTarea * b);

// Implementacion

void inicializarListaTareas(tListaTareas * lista) {
    lista->inicio = NULL;
}

bool listaTareasVacia(tListaTareas lista) {

    // true si la lista no tiene nodos, false si contiene al menos una tarea.

    bool vacia;
    vacia = (lista.inicio == NULL);
    return vacia;
}

void agregarTareaLista(tListaTareas * lista, tDatosTarea tarea) {
    tNodoListaTarea * nuevoNodo;
    tNodoListaTarea * aux;
    bool vacia;

    nuevoNodo = (tNodoListaTarea *) malloc(sizeof(tNodoListaTarea));

    if (nuevoNodo == NULL) {
        printf("No hay memoria para agregar a la lista.\n");
    } else {
        nuevoNodo->tarea = tarea;
        nuevoNodo->siguiente = NULL;

        vacia = listaTareasVacia(*lista);
        if (vacia) {
            lista->inicio = nuevoNodo;
        } else {
            aux = lista->inicio;
            while (aux->siguiente != NULL) {
                aux = aux->siguiente;
            }
            aux->siguiente = nuevoNodo;
        }
    }
}

void mostrarListaTareas(tListaTareas lista) {
    tNodoListaTarea * aux;

    aux = lista.inicio;

    if (aux == NULL) {
        printf("\n(No hay tareas en la lista)\n");
    } else {
        while (aux != NULL) {
            mostrarTarea(aux->tarea);
            aux = aux->siguiente;
        }
    }
}

void cargarListaDesdeColas(tListaTareas * lista, tCola * alta, tCola * media, tCola * baja) {
    tNodoColaPrioridad * aux;

    // recorrer cola ALTA
    aux = alta->frente;
    while (aux != NULL) {
        agregarTareaLista(lista, aux->tarea);
        aux = aux->siguiente;
    }

    // recorrer cola MEDIA
    aux = media->frente;
    while (aux != NULL) {
        agregarTareaLista(lista, aux->tarea);
        aux = aux->siguiente;
    }

    // recorrer cola BAJA
    aux = baja->frente;
    while (aux != NULL) {
        agregarTareaLista(lista, aux->tarea);
        aux = aux->siguiente;
    }
}

void buscarTareasPorEtiqueta(tListaTareas lista, tStringEtiqueta etiqueta) {
    tNodoListaTarea * aux;
    bool encontrada;

    encontrada = false; // indica si encontramos alguna coincidencia
    aux = lista.inicio;

    printf("\n=== RESULTADOS DE BUSQUEDA POR ETIQUETA ===\n");

    while (aux != NULL) {
        if (strcmp(aux->tarea.etiqueta, etiqueta) == 0) {
            mostrarTarea(aux->tarea);
            encontrada = true;
        }
        aux = aux->siguiente;
    }

    if (!encontrada) {
        printf("No se encontraron tareas con esa etiqueta.\n");
    }
}

void ordenarListaPorFecha(tListaTareas * lista) {
    lista->inicio = mergeSortLista(lista->inicio);
}

void vaciarListaTareas(tListaTareas * lista) {
    tNodoListaTarea * borrar;
    bool vacia;

    vacia = listaTareasVacia(*lista);
    while (!vacia) {
        borrar = lista->inicio;
        lista->inicio = borrar->siguiente;
        free(borrar);
        vacia = listaTareasVacia(*lista);
    }
}


// Funciones del Merge Sort para ordenar la lista por fecha.

/* esta función aplica Merge Sort sobre la lista de tareas.
Si la lista tiene 0 o 1 nodo, ya está ordenada.
Si tiene más, se divide en dos mitades, se ordena cada una
por separado de forma recursiva y luego se fusionan.
Devuelve el puntero al inicio de la lista ya ordenada. */

tNodoListaTarea * mergeSortLista(tNodoListaTarea * inicio) {
    tNodoListaTarea * mitad1;
    tNodoListaTarea * mitad2;

    if (inicio == NULL || inicio->siguiente == NULL) {
        return inicio;
    }

    dividirLista(inicio, &mitad1, &mitad2);

    mitad1 = mergeSortLista(mitad1);
    mitad2 = mergeSortLista(mitad2);

    return mezclarListasPorFecha(mitad1, mitad2);
}

/* divide la lista en dos partes aproximadamente iguales.
Usa dos punteros: "lento" avanza de a 1, "rápido" de a 2.
Cuando "rápido" llega al final, "lento" queda en el medio.
mitad1 apunta al primer tramo, mitad2 al segundo.
Esta función no ordena, solo separa la lista para el merge sort. */

void dividirLista(tNodoListaTarea * origen, tNodoListaTarea ** mitad1, tNodoListaTarea ** mitad2) {
    tNodoListaTarea * lento;
    tNodoListaTarea * rapido;
    bool seguir;

    if (origen == NULL || origen->siguiente == NULL) {
        *mitad1 = origen;
        *mitad2 = NULL;
    } else {
        lento = origen;
        rapido = origen->siguiente;
        seguir = true;

        while (seguir) {
            if (rapido == NULL) {
                seguir = false;
            } else {
                rapido = rapido->siguiente;
                if (rapido == NULL) {
                    seguir = false;
                } else {
                    lento = lento->siguiente;
                    rapido = rapido->siguiente;
                }
            }
        }

        *mitad1 = origen;
        *mitad2 = lento->siguiente;
        lento->siguiente = NULL;
    }
}

/* fusiona dos listas ya ordenadas por fecha en una sola lista ordenada.
Compara las fechas de los primeros nodos de cada lista
y elige el que corresponda para poner primero.
Se llama a sí misma recursivamente hasta unir toda la lista. */

tNodoListaTarea * mezclarListasPorFecha(tNodoListaTarea * a, tNodoListaTarea * b) {
    tNodoListaTarea * resultado;

    if (a == NULL) return b;
    if (b == NULL) return a;

    if (compararFechas(a->tarea.fecha, b->tarea.fecha) <= 0) {
        resultado = a;
        resultado->siguiente = mezclarListasPorFecha(a->siguiente, b);
    } else {
        resultado = b;
        resultado->siguiente = mezclarListasPorFecha(a, b->siguiente);
    }

    return resultado;
}

#endif // fin del ifndef.
