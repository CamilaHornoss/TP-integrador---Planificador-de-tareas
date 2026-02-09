/* --------------------------------------------------------
   Esta parte contiene las estructuras y funciones necesarias 
   para manejar una tarea: datos básicos, carga por teclado, 
   muestra por pantalla y comparación de fechas. 
   Se usa como base para colas, pilas y listas.
-------------------------------------------------------- */

#ifndef TAD_TAREA_H // este ifndef lo pusimos porque protege al archivo de ser incluido más de una vez.
#define TAD_TAREA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Constantes
#define MAX_TITULO 50
#define MAX_DESCRIPCION 200
#define MAX_ETIQUETA 30

// Tipos de datos personalizados

typedef char tStringTitulo[MAX_TITULO];
typedef char tStringDescripcion[MAX_DESCRIPCION];
typedef char tStringEtiqueta[MAX_ETIQUETA];

typedef struct {
    int dia;
    int mes;
    int anio;
} tDatosFecha;

typedef struct {
    int id;
    tStringTitulo titulo;
    tStringDescripcion descripcion;
    tStringEtiqueta etiqueta; // vamos a pedirle al usuario que ingrese una palabra clave para busqueda.
    int prioridad;   // vamos a definir tres, 1 = alta, 2 = media, 3 = baja. 
    int estado;      // vamos a definir 2, 0 = pendiente, 1 = completada.
    tDatosFecha fecha;
} tDatosTarea;

// Prototipos 
void cargarTarea(tDatosTarea * tarea, int idNueva);
void mostrarTarea(tDatosTarea tarea);
int compararFechas(tDatosFecha fecha1, tDatosFecha fecha2);

// Implementacion 
void cargarTarea(tDatosTarea * tarea, int idNueva) {
    int prioridad;

    tarea->id = idNueva;

    printf("\n=== NUEVA TAREA ===\n");
	
    printf("Titulo: ");
    fflush(stdin);
    fgets(tarea->titulo, MAX_TITULO, stdin);
    tarea->titulo[strcspn(tarea->titulo, "\n")] = '\0';

    printf("Descripcion: ");
    fflush(stdin);
    fgets(tarea->descripcion, MAX_DESCRIPCION, stdin);
    tarea->descripcion[strcspn(tarea->descripcion, "\n")] = '\0';

    printf("Etiqueta (una palabra): ");
    fflush(stdin);
    fgets(tarea->etiqueta, MAX_ETIQUETA, stdin);
    tarea->etiqueta[strcspn(tarea->etiqueta, "\n")] = '\0'; // el fgets guarda también el salto de línea \n. Con strcspn buscamos esa posición  
															// y la reemplazamos por \0 para dejar el string limpio y evitar errores en comparaciones. 
    
    printf("Prioridad (1-Alta, 2-Media, 3-Baja): ");
    scanf("%d", &prioridad);
    getchar();

    if (prioridad < 1 || prioridad > 3) {
        prioridad = 3;
    }
    tarea->prioridad = prioridad;

    tarea->estado = 0; // todas las tareas nuevas comienzan como pendientes

    printf("Fecha limite (dd mm aaaa, CON ESPACIOS) : ");
    scanf("%d %d %d", &tarea->fecha.dia, &tarea->fecha.mes, &tarea->fecha.anio);
    getchar();
}

void mostrarTarea(tDatosTarea tarea) {
    char * txtPrioridad;
    char * txtEstado;

    if (tarea.prioridad == 1) {
        txtPrioridad = "ALTA";
    } else {
        if (tarea.prioridad == 2) {
            txtPrioridad = "MEDIA";
        } else {
            txtPrioridad = "BAJA";
        }
    }

    if (tarea.estado == 0) {
        txtEstado = "Pendiente";
    } else {
        txtEstado = "Completa";
    }

    printf("\n-----------------------------\n");
    printf("ID: %d\n", tarea.id);
    printf("Titulo: %s\n", tarea.titulo);
    printf("Descripcion: %s\n", tarea.descripcion);
    printf("Etiqueta: %s\n", tarea.etiqueta);
    printf("Prioridad: %s\n", txtPrioridad);
    printf("Estado: %s\n", txtEstado);
    printf("Fecha limite: %02d/%02d/%04d\n",
           tarea.fecha.dia, tarea.fecha.mes, tarea.fecha.anio);
    printf("-----------------------------\n");
}

// decidimos hacer que <0 si fecha1<fecha2, 0 si es igual, >0 si fecha1>fecha2 
int compararFechas(tDatosFecha fecha1, tDatosFecha fecha2) {
    int resultado;

    resultado = fecha1.anio - fecha2.anio;
    if (resultado == 0) {
        resultado = fecha1.mes - fecha2.mes;
        if (resultado == 0) {
            resultado = fecha1.dia - fecha2.dia;
        }
    }
    return resultado;
}

#endif // fin del ifndef. 
