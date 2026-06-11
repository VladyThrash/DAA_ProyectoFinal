/*
Módulo 1:
    Leer mensajes cifrados y listarlos por prioridad DESC, fecha ASC, remitente ASC o longitud ASC (Minería).
	    *Archivos: mensajes_cifrados.txt y remitentes.txt
	    *Complejidad:  O(n log n) y almenos un O(n + m)
	    *Nota: Para los cuatro algoritmos deben de ser distintos. Deben de complir la complejidad anterior. 
*/

#ifndef Message_Analysis
#define Message_Analysis

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Leer_Numero.h" //Es solo una función de lectura segura de números enteros.

//Toma el control del flujo del programa desde main.
void message_analysis_menu();


void message_analysis_menu(){
    int input = 0;
    do{
        printf("\n--- LISTAR MENSAJES CIFRADOS EN ---\n");
        printf("[1]. Prioridad orden DESC...\n");
        printf("[2]. Fecha orden ASC...\n");
        printf("[3]. Remitente orden ASC...\n");
        printf("[4]. Longitud mensage orden ASC...\n");
        printf("[0]. Salir al menu principal...\n");
        printf("entrada: ");
        input = leer_numero(); //Obtenemos la entrada de forma segura.

        switch(input){
            case 1:
                
                break;

            case 2:
                
                break;

            case 3:
                
                break;

            case 4:
                
                break;

            case 0:
                printf("SALIENDO AL MENU PRINCIPAL...\n");
                break;
            
            default:
                printf("ENTRADA INVALIDA...\n");
                break;
        } 
    }
    while(input);
}

#endif