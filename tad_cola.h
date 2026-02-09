/* --------------------------------------------------------
   Esta parte define la estructura de cola y las operaciones básicas
   para manejar tareas en orden FIFO según su prioridad.
   Se usa para las colas de prioridad alta, media y baja 
   dentro del planificador de tareas.
-------------------------------------------------------- */

#ifndef TAD_COLA_H // este ifndef lo pusimos porque protege al archivo de ser incluido más de una vez.
#define TAD_COLA_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tad_tarea.h" // importa las estructuras y funciones del TAD Tarea necesarias para almacenar y manejar cada tarea.

// Definicion de la cola 

typedef struct nodoCola {
    tDatosTarea tarea;
    struct nodoCola * siguiente;
} tNodoColaPrioridad;

typedef struct {
    tNodoColaPrioridad * frente;
    tNodoColaPrioridad * final;
} tCola;

// Prototipos

void inicializarColaPrioridad(tCola * cola);
bool colaPrioridadVacia(tCola cola);
void agregarTareaCola(tCola * cola, tDatosTarea tarea);
bool quitarTareaCola(tCola * cola, tDatosTarea * tareaQuitada);
void mostrarColaPrioridad(tCola cola);
bool eliminarTareaPorIdCola(tCola * cola, int id, tDatosTarea * tareaEliminada);
bool cambiarEstadoTareaCola(tCola * cola, int id, int nuevoEstado, tDatosTarea * antes, tDatosTarea * despues);
void vaciarColaPrioridad(tCola * cola);

// Implementación

void inicializarColaPrioridad(tCola * cola) {
    cola->frente = NULL;
    cola->final = NULL;
}

bool colaPrioridadVacia(tCola cola) { 

	// devuelve true si la cola no tiene nodos, false si contiene al menos una tarea.
	
    bool vacia;
    vacia = (cola.frente == NULL);
    return vacia;
}

void agregarTareaCola(tCola * cola, tDatosTarea tarea) {
    tNodoColaPrioridad * nuevoNodoTarea;
    bool vacia;
	
	nuevoNodoTarea = (tNodoColaPrioridad *) malloc(sizeof(tNodoColaPrioridad));
	
    if (nuevoNodoTarea == NULL) {
        printf("No hay memoria para agregar la tarea a la cola.\n");
    } else {
        nuevoNodoTarea->tarea = tarea;
        nuevoNodoTarea->siguiente = NULL;

        vacia = colaPrioridadVacia(*cola);
        if (vacia) {
            cola->frente = nuevoNodoTarea;
            cola->final = nuevoNodoTarea;
        } else {
            cola->final->siguiente = nuevoNodoTarea;
            cola->final = nuevoNodoTarea;
        }
    }
}

bool quitarTareaCola(tCola * cola, tDatosTarea * tareaQuitada) {
	
	// devuelve true si se pudo quitar una tarea, false si la cola está vacía.
	
    bool vacia; 
    tNodoColaPrioridad * borrarNodoTarea;

    vacia = colaPrioridadVacia(*cola);
    if (vacia) {
        return false;
    } else {
        borrarNodoTarea = cola->frente;
        *tareaQuitada = borrarNodoTarea->tarea;
        cola->frente = borrarNodoTarea->siguiente;
        if (cola->frente == NULL) {
            cola->final = NULL;
        }
        free(borrarNodoTarea);
        return true;
    }
}

void mostrarColaPrioridad(tCola cola) {
    tNodoColaPrioridad * aux;

    aux = cola.frente;
    if (aux == NULL) {
        printf("\n(No hay tareas en esta cola de prioridad)\n");
    } else {
        while (aux != NULL) {
            mostrarTarea(aux->tarea);
            aux = aux->siguiente;
        }
    }
}

bool eliminarTareaPorIdCola(tCola * cola, int id, tDatosTarea * tareaEliminada) { 

	// retorna true si se encontró una tarea con ese ID y se eliminó.
	// retorna false si no existe ninguna tarea con ese ID en la cola.
	
	
    tNodoColaPrioridad * nodoTareaActual;
    tNodoColaPrioridad * nodoTareaAnt;
    bool encontrada; 

    nodoTareaActual = cola->frente;
    nodoTareaAnt = NULL;
    encontrada = false;

    while (nodoTareaActual != NULL && !encontrada) {
        if (nodoTareaActual->tarea.id == id) {
            encontrada = true;
        } else {
            nodoTareaAnt = nodoTareaActual;
            nodoTareaActual = nodoTareaActual->siguiente;
        }
    }

    if (!encontrada) {
        return false; 
    }

    if (nodoTareaAnt == NULL) {
        cola->frente = nodoTareaActual->siguiente;
    } else {
        nodoTareaAnt->siguiente = nodoTareaActual->siguiente;
    }

    if (nodoTareaActual == cola->final) {
        cola->final = nodoTareaAnt;
    }

    *tareaEliminada = nodoTareaActual->tarea;
    free(nodoTareaActual);

    return true; 
}

bool cambiarEstadoTareaCola(tCola * cola, int id, int nuevoEstado, tDatosTarea * antes, tDatosTarea * despues) {
	
	//retorna true si se encontró la tarea, se cambió su estado y se guardaron los valores antes/después.
	//retorna false si no existe tarea con ese ID.
	
    tNodoColaPrioridad * NodoTareaActual;
    bool encontrada;

    NodoTareaActual = cola->frente;
    encontrada = false;

    while (NodoTareaActual != NULL && !encontrada) {
        if (NodoTareaActual->tarea.id == id) {
            encontrada = true;
        } else {
            NodoTareaActual = NodoTareaActual->siguiente;
        }
    }

    if (!encontrada) {
        return false;
    }

    *antes = NodoTareaActual->tarea;
    NodoTareaActual->tarea.estado = nuevoEstado;
    *despues = NodoTareaActual->tarea;

    return true;
}

void vaciarColaPrioridad(tCola * cola) {
    tDatosTarea aux;
    bool sePuedeQuitar; // variable bandera: indica si se pudo quitar una tarea. Permite repetir el proceso hasta dejar la cola vacía.

    sePuedeQuitar = true;
    while (sePuedeQuitar) {
        sePuedeQuitar = quitarTareaCola(cola, &aux);
    }
}

#endif // fin del ifndef.
