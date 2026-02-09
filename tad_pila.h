/* --------------------------------------------------------
   Esta parte define la estructura de pila y las operaciones
   básicas para manejar acciones (Undo/Redo) en orden LIFO.
   Cada nodo guarda una operación realizada sobre una tarea.
-------------------------------------------------------- */

#ifndef TAD_PILA_H   // este ifndef protege al archivo de ser incluido más de una vez
#define TAD_PILA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tad_tarea.h"   // necesitamos tDatosTarea para guardar la tarea antes y después de la acción

/* 
   Tipo de operación:
   1 = agregar tarea
   2 = eliminar tarea
   3 = completar tarea
*/

// Definicion de la pila
typedef struct {
    int tipo;              // tipo de acción realizada
    tDatosTarea antes;     // estado de la tarea ANTES de la acción
    tDatosTarea despues;   // estado de la tarea DESPUÉS de la acción
} tDatosOperacion;

typedef struct nodoPilaAccion {
    tDatosOperacion operacion;
    struct nodoPilaAccion * siguiente;
} tNodoPilaAccion;

typedef tNodoPilaAccion * tPilaAcciones;

// Prototipos

void inicializarPilaAcciones(tPilaAcciones * pila);
bool pilaAccionesVacia(tPilaAcciones pila);
void apilarAccion(tPilaAcciones * pila, tDatosOperacion operacion);
bool desapilarAccion(tPilaAcciones * pila, tDatosOperacion * operacion);
void vaciarPilaAcciones(tPilaAcciones * pila);

// Implementación

void inicializarPilaAcciones(tPilaAcciones * pila) {
    *pila = NULL;
}

bool pilaAccionesVacia(tPilaAcciones pila) {

    // devuelve true si no hay nodos en la pila (pila == NULL),
    // false si hay al menos una operación apilada.
    
    bool vacia;
    vacia = (pila == NULL);
    return vacia;
}

void apilarAccion(tPilaAcciones * pila, tDatosOperacion operacion) {
    tNodoPilaAccion * nuevoNodoAccion;

    nuevoNodoAccion = (tNodoPilaAccion *) malloc(sizeof(tNodoPilaAccion));

    if (nuevoNodoAccion == NULL) {
        printf("No hay memoria para apilar la accion.\n");
    } else {
        nuevoNodoAccion->operacion = operacion;
        nuevoNodoAccion->siguiente = *pila;   
        *pila = nuevoNodoAccion;              
    }
}

bool desapilarAccion(tPilaAcciones * pila, tDatosOperacion * operacion) {
    
    // devuelve true si se pudo desapilar una operacion,
    // false si la pila estaba vacía y no hay nada para sacar.
    
    bool vacia;
    tNodoPilaAccion * borrarNodoAccion;

    vacia = pilaAccionesVacia(*pila);
    if (vacia) {
        return false;
    } else {
        borrarNodoAccion = *pila;
        *operacion = borrarNodoAccion->operacion;   
        *pila = borrarNodoAccion->siguiente;       
        free(borrarNodoAccion);
        return true;
    }
}

void vaciarPilaAcciones(tPilaAcciones * pila) {
    tDatosOperacion aux;
    bool sePuedeDesapilar;   // variable bandera: indica si se pudo desapilar una operación

    sePuedeDesapilar = true;
    while (sePuedeDesapilar) {
        sePuedeDesapilar = desapilarAccion(pila, &aux);
    }
}

#endif // fin del ifndef.
