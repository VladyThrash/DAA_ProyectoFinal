/*
Módulo 3:
    Analizar frecuencias de letras y proponer equivalencias probables para deducir una palabra objetivo (Descifrado y Análisis):
        *Archivos: messages_cifrados.txt, frecuencias.txt y palabras_frecuentes.txt
        *Complejidad: O(L + k log k + p)
        *Nota: La palabra debe de estar cifrada, la palabra objetivo sale de palabras_frecuentes.txt que si esta descifrada.
*/

/*
PROPUESTA:
    1.- Tokenizar el message y listar las palabras para seleccionar una individualmente.
    2.- Obtener la frecuencia de caracteres para la palabra cifrada especifica.
    3.- Ordenar la frecuencia de caracteres de la palabra cifrada en orden descendente.
    4.- Ordenar la frecuencia del archivo frecuencias.txt en orden descendente.
    5.- Crear una tabla de sustitucion en base a las frecuencias detectadas.
    6.- Aplicar el descifrado a la palabra con la tabla de sustitucion creada.
    7.- Comparar la distancia lexicometrica de la palabra descifrada con respecto a las palabras del diccionario.
    8.- Si no hay mucha diferencia, la palabra comparada es candidata.
*/

#ifndef Word_Inference
#define Word_Inference

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Leer_Numero.h" //Es solo una función de lectura segura de números enteros.

//Structs para vaciar la información de los archivos.
struct message{
    char id[20];
    char remitente[50];
    char prioridad[10];
    char fecha[20];
    char clave_id[20];
    char texto_original[256];
    char texto_cifrado[256];
};

struct frecuencia{
    char letra;
    float freq;
};

struct palabra{
    char word[50];
};

//Paths a los archivos txt.
char path_messages[] = "Cryptonituv_DB/mensajes_cifrados.txt";
char path_freqs[] = "Cryptonituv_DB/frecuencias.txt";
char path_words[] = "Cryptonituv_DB/palabras_frecuentes.txt";

//Punteros para crear arreglo de structs.
struct message *messages_inf = NULL;
struct frecuencia *freqs = NULL;
struct palabra *words = NULL;

//Toma el control del flujo del programa desde main.
void word_inference_menu();

//Inicializa la lectura de los archivos txt y extrae la información en los structs.
int extraer_messages_inf_txt();
int extraer_freqs_txt();
int extraer_words_txt();

//Imprimir la información contenida en el arreglo.
void imprimir_messages_inf(int n);

//Procesa el texto cifrado, lo tokeniza y permite al usuario elegir una sola palabra.
void seleccionar_palabra(struct message *msg, int num_freqs, int num_words);

//Aplica los Pasos del 2 al 8 para buscar candidatas de la palabra ingresada.
//Complejidad: O(L + k log k + p)
void inferir_palabra(char *palabra_cifrada, int num_freqs, int num_words);

//Ordena en DESC las frecuencias utilizando Quick Sort.
//Complejidad: O(k*log(k)) para caso promedio.
void quick_sort_freq(struct frecuencia *arr, int izq, int der);
int partition_freq(struct frecuencia *arr, int izq, int der);
void swap_freq(struct frecuencia *f1, struct frecuencia *f2);

//Funciones auxiliares para inferencia.
int obtener_frecuencias_palabra(char *palabra, struct frecuencia *freqs_palabra, int len);
void crear_mapa_sustitucion(char *mapa, struct frecuencia *f_palabra, struct frecuencia *f_idioma, int limit_idioma);
void descifrar_palabra(char *cifrada, char *descifrada, char *mapa, int len);
void buscar_candidatas(char *palabra_descifrada, int len, int num_words);


void word_inference_menu(){
    int num_messages = extraer_messages_inf_txt(); //Leemos los messages desde el txt.
    int num_freqs = extraer_freqs_txt(); //Leemos las frecuencias desde el txt.
    int num_words = extraer_words_txt(); //Leemos las palabras desde el txt.
    
    if(num_messages == 0 || num_freqs == 0 || num_words == 0){ //Validar que se extrajo la información de los archivos txt.
        return;
    }
    
    int input = 0;
    do{
        printf("\n--- INFERENCIA DE PALABRAS ---\n");
        printf("[1]. Seleccionar un message para analisis...\n");
        printf("[0]. Salir al menu principal...\n");
        printf("entrada: ");
        input = leer_numero(); //Obtenemos la entrada de forma segura.

        switch(input){
            case 1:
                printf("\n--- LISTADO messageS CIFRADOS ---\n");
                imprimir_messages_inf(num_messages);
                printf("\nIngresa el ID para seleccionar el message: ");
                int sel = leer_numero();
                
                if(sel >= 1 && sel <= num_messages){
                    seleccionar_palabra(&messages_inf[sel - 1], num_freqs, num_words);
                } else {
                    printf("ENTRADA INVALIDA...\n");
                }
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

    free(messages_inf);
    free(freqs);
    free(words);
}

void seleccionar_palabra(struct message *msg, int num_freqs, int num_words){
    printf("\n--- message SELECCIONADO ---\n%s\n", msg->texto_cifrado);
    
    char buffer_texto[256];
    strcpy(buffer_texto, msg->texto_cifrado); 
    
    char lista_palabras[100][50]; //Arreglo temporal para guardar palabras cifradas.
    int total_palabras = 0;

    //Tokenizar cada palabra y guardarla en un arreglo temporal para selección.
    char *token = strtok(buffer_texto, " ,.-;:\n");
    while(token != NULL && total_palabras < 100){
        strcpy(lista_palabras[total_palabras], token);
        total_palabras++;
        token = strtok(NULL, " ,.-;:\n"); 
    }

    if(total_palabras == 0){
        printf("El message no contiene palabras legibles.\n");
        return;
    }

    printf("\nPalabras detectadas en el message:\n");
    for(int i = 0; i < total_palabras; i++){
        printf("[%d]. %s\n", i + 1, lista_palabras[i]);
    }

    printf("\nSelecciona el numero de la palabra a inferir: ");
    int sel_palabra = leer_numero(); //Obtenemos la entrada de forma segura.

    if(sel_palabra >= 1 && sel_palabra <= total_palabras){
        printf("\n--------------------------------------------------\n");
        printf("Evaluando palabra cifrada: '%s'\n", lista_palabras[sel_palabra - 1]);
        inferir_palabra(lista_palabras[sel_palabra - 1], num_freqs, num_words);
        printf("--------------------------------------------------\n");
    } else {
        printf("SELECCION INVALIDA...\n");
    }
}

void inferir_palabra(char *palabra_cifrada, int num_freqs, int num_words){
    int len = strlen(palabra_cifrada); //Obtenemos la longitud de la palabra.
    if(len == 0){ //La palabra es nula.
        return;
    }

    //Obtener la frecuencia de caracteres. Complejidad: O(L).
    struct frecuencia freqs_palabra[26];
    int validas = obtener_frecuencias_palabra(palabra_cifrada, freqs_palabra, len);
    if(validas == 0){ //Validar que se hallan contibilizado caracteres.
        printf("Palabra sin caracteres validos en el alfabeto base.\n");
        return;
    }
    
    //Ordenar frecuencias de la palabra cifrada. Complejidad: O(k log k)
    quick_sort_freq(freqs_palabra, 0, 25);
    
    //Ordenar frecuencias reales del idioma. Complejidad: O(k log k).
    //Creamos una copia para no alterarla.
    struct frecuencia *freqs_idioma = (struct frecuencia*)malloc(sizeof(struct frecuencia) * num_freqs);
    for(int i = 0; i < num_freqs; i++){
        freqs_idioma[i] = freqs[i];
    }
    quick_sort_freq(freqs_idioma, 0, num_freqs - 1); 
    
    //Crear mapa de sustitución de caracteres basado en frecuencias.
    char *mapa = (char*)malloc(sizeof(char) * 256);
    crear_mapa_sustitucion(mapa, freqs_palabra, freqs_idioma, num_freqs);
    
    //Descifrar palabra: Complejidad: O(L).
    char palabra_descifrada[50];
    descifrar_palabra(palabra_cifrada, palabra_descifrada, mapa, len);
    printf("Posible descifrado: %s\n", palabra_descifrada);
    
    //Buscar candidatas por distancia lexicometrica. Complejidad: O(p).
    buscar_candidatas(palabra_descifrada, len, num_words);
    
    free(mapa);
    free(freqs_idioma);
}

int obtener_frecuencias_palabra(char *palabra, struct frecuencia *f_palabra, int len){
    for(int i = 0; i < 26; i++){
        f_palabra[i].letra = 'a' + i; //Va generando las letras del abcedario.
        f_palabra[i].freq = 0;
    }
    int cont = 0;

    //Coonteo letra por letra.
    for(int i = 0; i < len; i++){
        char c = palabra[i];
        if(c >= 'a' && c <= 'z'){
            f_palabra[c - 'a'].freq += 1;
            cont++;
        }
    }
    return cont;
}

void crear_mapa_sustitucion(char *mapa, struct frecuencia *f_palabra, struct frecuencia *f_idioma, int limit_idioma){
    //Se mapean los caracteres ASCII.
    for(int i = 0; i < 256; i++) {
        mapa[i] = (char)i; //type cast.
    }
    
    //Asigna equivalencias por frecuencia: recorre el alfabeto cifrado y, si la letra existe en la palabra, le asigna la siguiente 
    //letra más probable del idioma.
    int map_idx = 0;
    for(int i = 0; i < 26; i++){
        if(f_palabra[i].freq > 0 && map_idx < limit_idioma){
            mapa[f_palabra[i].letra] = f_idioma[map_idx].letra;
            map_idx++;
        }
    }
}

void descifrar_palabra(char *cifrada, char *descifrada, char *mapa, int len){
    strcpy(descifrada, cifrada); //Copiamos para mantener integridad al descifrar.
    
    //Desciframos el message utilizando el mapa de carateres.
    for(int i = 0; i < len; i++){ 
        descifrada[i] = mapa[cifrada[i]];
    }
}

void buscar_candidatas(char *palabra_descifrada, int len, int num_words){
    printf("\nBuscando coincidencias cercanas en diccionario...\n");
    int encontradas = 0;
    
    for(int i = 0; i < num_words; i++){
        if(strlen(words[i].word) == len){ //Primer filtro: Es del mismo tamaño.
            int dist = strcmp(palabra_descifrada, words[i].word); //Obtenemos distancia lexicometrica.
            if(dist >= -2 && dist <= 2){ //Segundo filtro: Intervalo de confianza.
                printf(" -> [CANDIDATA] %s (Diferencia posicional: %d)\n", words[i].word, dist);
                encontradas++;
            }
        }
    }
    
    if(encontradas == 0){
        printf(" -> Ninguna coincidencia cercana encontrada en diccionario\n");
    }
}

void quick_sort_freq(struct frecuencia *arr, int izq, int der){
    if(izq >= der){
        return;
    }
        
    int p = partition_freq(arr, izq, der); 
    quick_sort_freq(arr, izq, p - 1); //Subarreglo izquierdo.
    quick_sort_freq(arr, p + 1, der); //Subarreglo derecho.
}

int partition_freq(struct frecuencia *arr, int izq, int der){
    int i = izq, j = der - 1; //Para iterar en ambos sentidos.
    struct frecuencia *pivote = &arr[der]; //último
    
    while(i <= j){
        while(arr[i].freq > pivote->freq){ //aqui no hay que poner i <=right, porque la condicion se rompe al compararse con el pivote
            i++; //Encontramos el indice del primer elemento mayor a pivote.
        }
        while(j >= izq && arr[j].freq < pivote->freq){ //Condicion para DESC.
            j--; //Encontramos el indice del primer elemento menor a pivote.
        }
        if(i <= j){
            swap_freq(&arr[i], &arr[j]); //Ambos se ordenan con respecto al pivote: menor -> subarreglo izquierdo, mayor -> derecho.
            i++; 
            j--;
        }
    }
    
    swap_freq(&arr[j + 1], &arr[der]); //Colocamos el pivote en el limite de los subarreglos.
    return j + 1; //Retornamos el indice del pivote.
}

void swap_freq(struct frecuencia *f1, struct frecuencia *f2){
    struct frecuencia aux = *f1;
    *f1 = *f2;
    *f2 = aux;
}

int extraer_messages_inf_txt(){
    FILE *archivo = fopen(path_messages, "r");
    if (archivo == NULL){
        printf("ERROR: NO SE PUDO ABRIR EL ARCHIVO...\n");
        return 0;
    }

    char buffer[1024];
    int total_lineas = 0;

    if (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
            total_lineas++; 
        }
    }

    if(total_lineas == 0){
        fclose(archivo);
        return 0;
    }

    messages_inf = (struct message*)malloc(sizeof(struct message)*total_lineas);
    if(messages_inf == NULL){
        printf("ERROR: NO SE PUDO ASIGNAR MEMORIA...\n");
        fclose(archivo);
        return 0;
    }

    rewind(archivo); 
    fgets(buffer, sizeof(buffer), archivo); 

    for(int i = 0; fgets(buffer, sizeof(buffer), archivo) != NULL && i < total_lineas; i++){
        char *token = strtok(buffer, "|\n");
        if (token != NULL) {
            strcpy(messages_inf[i].id, token);
        }

        token = strtok(NULL, "|\n");
        if (token != NULL) {
            strcpy(messages_inf[i].remitente, token);
        }

        token = strtok(NULL, "|\n");
        if (token != NULL) {
            strcpy(messages_inf[i].prioridad, token);
        }

        token = strtok(NULL, "|\n");
        if (token != NULL) {
            strcpy(messages_inf[i].fecha, token);
        }

        token = strtok(NULL, "|\n");
        if (token != NULL) {
            strcpy(messages_inf[i].clave_id, token);
        }

        token = strtok(NULL, "|\n");
        if (token != NULL) {
            strcpy(messages_inf[i].texto_original, token);
        }

        token = strtok(NULL, "|\n");
        if (token != NULL) {
            strcpy(messages_inf[i].texto_cifrado, token);
        }
    }

    fclose(archivo); 
    return total_lineas;
}

int extraer_freqs_txt(){
    FILE *archivo = fopen(path_freqs, "r");
    if (archivo == NULL) {
        printf("ERROR: NO SE PUDO ABRIR EL ARCHIVO...\n");
        return 0;
    }

    char buffer[1024];
    int total_lineas = 0;

    if (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
            total_lineas++;
        }
    }

    if(total_lineas == 0) { 
        printf("EL ARCHIVO NO CONTIENE DATOS...\n");
        fclose(archivo); 
        return 0; 
    }

    freqs = (struct frecuencia*)malloc(sizeof(struct frecuencia)*total_lineas);
    if(freqs == NULL) { 
        printf("ERROR: NO SE PUDO ASIGNAR MEMORIA...\n");
        fclose(archivo); 
        return 0; 
    }

    rewind(archivo);
    fgets(buffer, sizeof(buffer), archivo); 

    for(int i = 0; fgets(buffer, sizeof(buffer), archivo) != NULL && i < total_lineas; i++){
        char *token = strtok(buffer, "|\n");
        if (token != NULL) {
            freqs[i].letra = token[0];
        }

        token = strtok(NULL, "|\n");
        if (token != NULL) {
            freqs[i].freq = atof(token);
        }
    }

    fclose(archivo);
    return total_lineas;
}

int extraer_words_txt(){
    FILE *archivo = fopen(path_words, "r");
    if (archivo == NULL) {
        printf("ERROR: NO SE PUDO ABRIR EL ARCHIVO...\n");
        return 0;
    }

    char buffer[1024];
    int total_lineas = 0;

    if (fgets(buffer, sizeof(buffer), archivo) != NULL) {
        while (fgets(buffer, sizeof(buffer), archivo) != NULL) {
            total_lineas++;
        }
    }

    if(total_lineas == 0) { 
        printf("EL ARCHIVO NO CONTIENE DATOS...\n");
        fclose(archivo); 
        return 0; 
    }

    words = (struct palabra*)malloc(sizeof(struct palabra)*total_lineas);
    if(words == NULL) { 
        printf("ERROR: NO SE PUDO ASIGNAR MEMORIA...\n");
        fclose(archivo); 
        return 0; 
    }

    rewind(archivo);
    fgets(buffer, sizeof(buffer), archivo); 

    for(int i = 0; fgets(buffer, sizeof(buffer), archivo) != NULL && i < total_lineas; i++){
        char *token = strtok(buffer, "\r\n"); 
        if (token != NULL) {
            strcpy(words[i].word, token);
        }
    }

    fclose(archivo);
    return total_lineas;
}

void imprimir_messages_inf(int n){
    printf("ID\t|\tTexto Cifrado\n");
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0; i < n; i++){
        printf("%s\t|\t%s\n", messages_inf[i].id, messages_inf[i].texto_cifrado);
    }
    printf("-----------------------------------------------------------------------------------------------------------------------------\n");
}

#endif