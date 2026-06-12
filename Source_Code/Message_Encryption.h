/*
Módulo 2:
    Cifrar y descifrar mens utilizando una tabla de sustitución letra por letra (Cifrado y Descifrado):
        *Archivos: mens_cifrados.txt  y claves.txt
        *Complejidad: O(L)
*/

#ifndef Message_Encryption
#define Message_Encryption

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "Leer_Numero.h" //Es solo una función de lectura segura de números enteros.

//Structs para vaciar la información de los archivos.
struct men{
	char id[20];
    char remitente[50];
    char prioridad[10];
    char fecha[20];
    char clave_id[20];
    char texto_original[256];
    char texto_cifrado[256];
};

struct clave{
    char clave_id[20];
    char nombre[50];
    char tipo[20];
    char alfabeto_original[100];
    char tabla_sustitucion[100];
};

//Paths a los archivos txt.
char path_mens[] = "Cryptonituv_DB/mensajes_cifrados.txt";
char path_claves[] = "Cryptonituv_DB/claves.txt";

//Punteros para crear los arreglos de structs.
struct men *mens = NULL;
struct clave *claves = NULL;

//Toma el control del flujo del programa desde main.
void message_encryption_menu();

//Nos lleva a un menu donde podemos seleccionar un mensaje original y una clave para cifrarlo.
void cifrar_menu(int num_mens, int num_claves);

//Nos lleva a un menu donde podemos seleccionar un mensaje cifrado y una clave para descifrarlo.
void descifrar_menu(int num_mens, int num_claves);

//Nos lleva a un menu donde podemos seleccionar una clave de cifrado - descifrado.
struct clave* tablas_menu(int num_claves);

//Aplica el cifrado al mensaje original. Ahí mismo imprime el resultado.
//Utiliza un mapa o tabla de caracteres que sustituye el caracter original por el caracter del cifrado.
//Complejidad: O(L).
void cifrado(struct men *msg, struct clave *key);

//Aplica el descifrado al mensaje cifrado. Ahí mismo imprime el resultado.
//Utiliza un mapa o tabla de caracteres que hace la sustitución inversa para contrarestar el cifrado.
//Complejidad: O(L).
void descifrado(struct men *msg, struct clave *key);

//Inicializa la lectura de los archivos txt y extrae la información en los structs correspondientes.
int extraer_mens_txt();
int extraer_claves_txt();

//Imprimir la información contenida en cada uno de los arreglos de structs.
void imprimir_mens_originales(int n);
void imprimir_mens_cifrados(int n);
void imprimir_claves(int n);

//Para copiar la información de un struct men en otro.
void copy_mensaje(struct men *dst, struct men *src);


void message_encryption_menu(){
    int num_mens = extraer_mens_txt(); //Leemos los mens desde el txt.
    int num_claves = extraer_claves_txt(); //Leemos las claves desde el txt.
    
    if(num_mens == 0 || num_claves == 0){ //Validar que se extrajo la información de los archivos txt.
        return;
    }
    
    int input = 0;
    do{
        printf("\n--- CIFRADO Y DESCIFRADO ---\n");
        printf("[1]. Escojer un mensaje para cifrar...\n");
        printf("[2]. Escojer un mensaje para descifrar...\n");
        printf("[0]. Salir al menu principal...\n");
        printf("entrada: ");
        input = leer_numero(); //Obtenemos la entrada de forma segura.

        switch(input){
            case 1:
                cifrar_menu(num_mens, num_claves); //Nos lleva a un menu de selección de mensaje y clave.
                break;
            case 2:
                descifrar_menu(num_mens, num_claves); //Nos lleva a un menu de selección de mensaje y clave.
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

void cifrar_menu(int num_mens, int num_claves){
    int input = 0;

    printf("\n--- LISTADO mens ORIGINALES ---\n");
    imprimir_mens_originales(num_mens);

    do{
        printf("\n--- CIFRADO ---\n");
        printf("[0]. Regresar al menu anterior...\n");
        printf("Ingresa el ID para seleccionar el mensaje: ");
        input = leer_numero(); //Obtenemos la entrada de forma segura.
        
        if(input >= 1 && input <= num_mens){
            struct men msg;
            copy_mensaje(&msg, &mens[input - 1]);
            struct clave *key = tablas_menu(num_claves);
            if(key == NULL){
                continue;
            }
            cifrado(&msg, key);
        }
        else if(input != 0){
            printf("ENTRADA INVALIDA...\n");
        }
        else{
            printf("REGRESANDO AL MENU ANTERIOR...\n");
        }
    }
    while(input);
}

void descifrar_menu(int num_mens, int num_claves){
    int input = 0;

    printf("\n--- LISTADO mens CIFRADOS ---\n");
    imprimir_mens_cifrados(num_mens);

    do{
        printf("\n--- DESCIFRADO ---\n");
        printf("[0]. Regresar al menu anterior...\n");
        printf("Ingresa el ID para seleccionar el mensaje: ");
        input = leer_numero(); //Obtenemos la entrada de forma segura.
        
        if(input >= 1 && input <= num_mens){
            struct men msg;
            copy_mensaje(&msg, &mens[input - 1]);
            struct clave *key = tablas_menu(num_claves);
            if(key == NULL){
                continue;
            }
            descifrado(&msg, key);
        }
        else if(input != 0){
            printf("ENTRADA INVALIDA...\n");
        }
        else{
            printf("REGRESANDO AL MENU ANTERIOR...\n");
        }
    }
    while(input);
}

struct clave *tablas_menu(int num_claves){
    int input = 0;

    printf("\n--- LISTADO CLAVES CIFRADO - DESCIFRADO ---\n");
    imprimir_claves(num_claves);

    do{
        printf("\n--- CLAVES ---\n");
        printf("[0]. Regresar al menu anterior...\n");
        printf("Ingresa el NUM para seleccionar la clave: ");
        input = leer_numero(); //Obtenemos la entrada de forma segura.

        if(input >= 1 && input <= num_claves){
            return &claves[input - 1];
        }
        else if(input != 0){
            printf("ENTRADA INVALIDA...\n");
        }
        else{
            printf("REGRESANDO AL MENU ANTERIOR...\n");
        }
    }
    while(input);
    return NULL;
}

void cifrado(struct men *msg, struct clave *key){
    printf("\nMensaje Original: %s\n", msg->texto_original);
    printf("Clave Cifrado: %s\n", key->tabla_sustitucion);
    
    //Crear un arreglo de mapeo para los caracteres ASCII.
    char *mapa = (char*)malloc(sizeof(char)*256);
    for(int i = 0; i < 256; i++){
        mapa[i] = (char)i; //type cast.
    }
    
    //Sustituimos en el mapa el caracter original por el caracter cifrado.
    int len_clave = strlen(key->tabla_sustitucion); //Que es la misma para el alfabeto original.
    for(int i = 0; i < len_clave; i++){
        mapa[key->alfabeto_original[i]] = key->tabla_sustitucion[i];
    }

    //Ciframos el mensaje utilizando el mapa de carateres.
    int len_mensaje = strlen(msg->texto_original);
    for(int i = 0; i < len_mensaje; i++){
        msg->texto_original[i] = mapa[msg->texto_original[i]]; //Como msg es una copia, lo sustituimos directamente.
    }

    printf("Mensaje Cifrado: %s\n", msg->texto_original);
    free(mapa);
}

void descifrado(struct men *msg, struct clave *key){
    printf("\nMensaje Cifrado: %s\n", msg->texto_cifrado);
    printf("Clave Cifrado: %s\n", key->tabla_sustitucion);
    
    //Crear un arreglo de mapeo para los caracteres ASCII.
    char *mapa = (char*)malloc(sizeof(char)*256);
    for(int i = 0; i < 256; i++){
        mapa[i] = (char)i; //type cast.
    }
    
    //Sustituimos en el mapa el caracter cifrado por el caracter original.
    int len_clave = strlen(key->tabla_sustitucion); //Que es la misma para el alfabeto original.
    for(int i = 0; i < len_clave; i++){
        mapa[key->tabla_sustitucion[i]] = key->alfabeto_original[i];
    }

    //Desciframos el mensaje utilizando el mapa de carateres.
    int len_mensaje = strlen(msg->texto_cifrado);
    for(int i = 0; i < len_mensaje; i++){
        msg->texto_cifrado[i] = mapa[msg->texto_cifrado[i]]; //Como msg es una copia, lo sustituimos directamente.
    }

    printf("Mensaje Descifrado: %s\n", msg->texto_cifrado);
    free(mapa);
}

int extraer_mens_txt(){
	//Abrimos el archivo en modo lectura ("r").
    FILE *archivo = fopen(path_mens, "r");
    if (archivo == NULL) {
        printf("ERROR: NO SE PUDO ABRIR EL ARCHIVO {%s}...\n", path_mens);
        return 0;
    }

	char buffer[1024];
    int total_lineas = 0; //Número total de registros.

	//Leer y descartar la primera linea
    if (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
            total_lineas++; //Contar el total de registros.
        }
    }

	if(total_lineas == 0){
        printf("EL ARCHIVO {%s} NO CONTIENE DATOS ...\n", path_mens);
        fclose(archivo);
        return 0;
    }

	//Crear el arreglo con el número total de registros.
    mens = (struct men*)malloc(sizeof(struct men)*total_lineas);
    if(mens == NULL){
        printf("ERROR: NO SE PUDO ASIGNAR MEMORIA...\n");
        fclose(archivo);
        return 0;
    }

	rewind(archivo); //Regresar el archivo al inicio.
    fgets(buffer, sizeof(buffer), archivo); //Descartar el encabezado.

	//Leer linea por linea y asignar a cada indice del arreglo los campos correspondientes.
	for(int i = 0; fgets(buffer, sizeof(buffer), archivo) != NULL && i < total_lineas; i++){
		char *token = strtok(buffer, "|\n");
        if (token != NULL) {
			strcpy(mens[i].id, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mens[i].remitente, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mens[i].prioridad, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mens[i].fecha, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mens[i].clave_id, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mens[i].texto_original, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mens[i].texto_cifrado, token);
		}
	}

	fclose(archivo); //Cerrar el archivo.
	return total_lineas;
}

int extraer_claves_txt(){
	//Abrimos el archivo en modo lectura ("r").
    FILE *archivo = fopen(path_claves, "r");
    if (archivo == NULL) {
        printf("ERROR: NO SE PUDO ABRIR EL ARCHIVO {%s}...\n", path_claves);
        return 0;
    }

	char buffer[1024];
    int total_lineas = 0; //Número total de registros.

	//Leer y descartar la primera linea
    if (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
            total_lineas++; //Contar el total de registros.
        }
    }

	if(total_lineas == 0){
        printf("EL ARCHIVO {%s} NO CONTIENE DATOS ...\n", path_claves);
        fclose(archivo);
        return 0;
    }

	//Crear el arreglo con el número total de registros.
    claves = (struct clave*)malloc(sizeof(struct clave)*total_lineas);
    if(claves == NULL){
        printf("ERROR: NO SE PUDO ASIGNAR MEMORIA...\n");
        fclose(archivo);
        return 0;
    }

	rewind(archivo); //Regresar el archivo al inicio.
    fgets(buffer, sizeof(buffer), archivo); //Descartar el encabezado.

	//Leer linea por linea y asignar a cada indice del arreglo los campos correspondientes.
	for(int i = 0; fgets(buffer, sizeof(buffer), archivo) != NULL && i < total_lineas; i++){
		char *token = strtok(buffer, "|\n"); //clave_id|nombre|tipo|alfabeto_original|tabla_sustitucion
        if (token != NULL) {
			strcpy(claves[i].clave_id, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(claves[i].nombre, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(claves[i].tipo, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(claves[i].alfabeto_original, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(claves[i].tabla_sustitucion, token);
		}
	}

	fclose(archivo); //Cerrar el archivo.
	return total_lineas;
}

void imprimir_mens_originales(int n){
	printf("ID\t|\tTexto Original\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	
	for(int i = 0; i < n; i++){
		printf("%s\t|\t%s\n", mens[i].id, mens[i].texto_original);
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
}

void imprimir_mens_cifrados(int n){
	printf("ID\t|\tClave ID\t|\tTexto Cifrado\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	
	for(int i = 0; i < n; i++){
		printf("%s\t|\t%s\t|\t%s\n", mens[i].id, mens[i].clave_id, mens[i].texto_cifrado);
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
}

void imprimir_claves(int n){
	printf("Num\t|\tClave ID\t|\tNombre\t|\tTipo\t|\tTabla Sustitucion\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	
	for(int i = 0; i < n; i++){
		printf("%d\t|\t%s\t|\t%s\t|\t%s\t|\t%s\n", i + 1, claves[i].clave_id, claves[i].nombre, claves[i].tipo, claves[i].tabla_sustitucion);
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
}

void copy_mensaje(struct men *dst, struct men *src){
    //Copiamos el contenido de src en dst.
    strcpy(dst->id, src->id);
    strcpy(dst->remitente, src->remitente);
    strcpy(dst->prioridad, src->prioridad);
    strcpy(dst->fecha, src->fecha);
    strcpy(dst->clave_id, src->clave_id);
    strcpy(dst->texto_original, src->texto_original);
    strcpy(dst->texto_cifrado, src->texto_cifrado);
}

#endif