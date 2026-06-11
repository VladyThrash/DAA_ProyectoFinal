#ifndef Leer_Numero
#define Leer_Numero

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//C es algo especial con las entradas, si estoy tratando de leer un entero y el usuario ingresa, por ejemplo, un caracter,
//el programa podria tener un comportamiento erratico e inclusive cerrar la ejecución. Esta función asegura la entrada de datos.
int leer_numero();

int leer_numero(){
    char buffer[100]; //Guarda la entrada del usuario.
    int numero;

    if(fgets(buffer, sizeof(buffer), stdin) != NULL) { //Lee la entrada completa de texto desde el teclado.
        if (sscanf(buffer, "%d", &numero) == 1) { //Intenta sacar un valor entero de la cadena.
            return numero; //Devuelve el número.
        }
    }
    return -1; //Regresa un valor fuera de rango para continuar el ciclo.
}

#endif