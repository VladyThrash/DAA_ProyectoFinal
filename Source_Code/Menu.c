//Contiene la función main con el menú y las llamadas a las librerías por módulo.
//Compilarlo como: "gcc Menu.c -o cifrador" para que gitignore lo detecte y no lo suba el ejecutable al repositorio.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

//Importamos todos nuestros módulos.
#include "Message_Analysis.h"
#include "Message_Encryption.h"
#include "Word_Inference.h"
#include "Top_Words.h"
#include "Word_Message_Analysis.h"
#include "Leer_Numero.h" //Es solo una función de lectura segura de números enteros.

//Contiene el switch case que dirije el flujo del programa al módulo especifico.
void menu();

//Contiene un arte ascii con el nombre el proyecto que se ejecuta al inicio del programa.
void arte_ascii();

int main(int argc, char const *argv[]){
    arte_ascii();
    menu();
    return 0;
}

void menu(){
    int input = 0;
    do{
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("[1]. Leer mensajes cifrados y listarlos...\n");
        printf("[2]. Cifrar y descifrar mensajes...\n");
        printf("[3]. Deducir una palabra cifrada...\n");
        printf("[4]. Mostrar las 10 palabras mas frecuentes...\n");
        printf("[5]. Mostrar las apariciones y equivalencias de una palabra...\n");
        printf("[0]. Salir\n");
        printf("entrada: ");
        input = leer_numero(); //Obtenemos la entrada de forma segura.

        switch(input){
            case 1:
                message_analysis_menu(); //El flujo continua desde Message_Analysis.h
                break;

            case 2:
                message_encryption_menu(); //El flujo continua desde Message_Encryption.h
                break;

            case 3:
                word_inference_menu(); //El flujo continua desde Word_Inference.h
                break;

            case 4:
                top_words_menu("Cryptonituv_DB/mensajes_cifrados.txt"); //El flujo continua desde Top_Words.h
                break;

            case 5:
                word_message_analysis_menu("Cryptonituv_DB/mensajes_cifrados.txt"); //El flujo continua desde Word_Message_Analysis.h
                break;

            case 0:
                printf("SALIENDO DEL PROGRAMA...\n");
                break;
            
            default:
                printf("ENTRADA INVALIDA...\n");
                break;
        } 
    }
    while(input);
}

void arte_ascii(){
    // \x1b[32m = Verde (Candado) | \x1b[36m = Cian (Texto) | \x1b[0m = Reset
    printf(
        "\x1b[32m"
        "                         .-----.\n"
        "                        /       \\\n"
        "                       |  .---.  |\n"
        "                       |  |   |  |\n"
        "                    .--|--|---|--|--.\n"
        "                    |  |  |   |  |  |\n"
        "                    |               |\n"
        "                    |      .-.      |\n"
        "                    |     (   )     |\n"
        "                    |      | |      |\n"
        "                    |      | |      |\n"
        "                    |      `-'      |\n"
        "                    |               |\n"
        "                    `---------------'\n"
        "\x1b[36m"
        "  _____                  _              _ _                _____  ____ \n"
        " / ____|                | |            (_) |              |  __ \\|  _ \\\n"
        "| |     _ __ _   _ _ __ | |_ ___  _ __  _| |_ _   ___   __| |  | | |_) |\n"
        "| |    | '__| | | | '_ \\| __/ _ \\| '_ \\| | __| | | \\ \\ / /| |  | |  _ < \n"
        "| |____| |  | |_| | |_) | || (_) | | | | | |_| |_| |\\ V / | |__| | |_) |\n"
        " \\_____|_|   \\__, | .__/ \\__\\___/|_| |_|_|\\__|\\__,_| \\_/  |_____/|____/\n"
        "              __/ | |                                                  \n"
        "             |___/|_|                                                  \n"
        "\x1b[0m\n"
    );
}