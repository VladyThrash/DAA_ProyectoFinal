/*
Módulo 5:
    Buscar una palabra y mostrar sus apariciones (Cuantas y donde) y equivalencias cifradas según cada palabra clave (Minería, Análisis y Cifrado):
        *Archivos: mensajes_cifrados y  claves.txt 
        *Complejidad: O(T + q)
        *Nota: Apariciones no cifradas y cifradas. La palabra es ingresada por el usuario.
*/

#ifndef Word_Message_Analysis
#define Word_Message_Analysis

typedef struct 
{
    char id[10];
    char sustitucion[30];
} Clave;

void word_message_analysis_menu(const char *ruta_archivo);
void leer_claves(Clave claves[], int *total_claves);
void cifrar_palabra(char *palabra, char *alfabeto, char *sustitucion, char *resultado);

// Función para cifrar una palabra usando un alfabeto y una sustitución dada mandando
// a llamar a strchr para encontrar la posición de cada letra en el alfabeto y 
// luego reemplazarla por la letra correspondiente en la sustitución. 
// Si la letra no se encuentra en el alfabeto, se deja igual y el resultado se 
// termina con un carácter nulo.
void cifrar_palabra(char *palabra, char *alfabeto, char *sustitucion, char *resultado) 
{
    for(int i = 0; i < strlen(palabra); i++) 
    {
        char *pos = strchr(alfabeto, palabra[i]);
        if (pos == NULL) 
        {
            resultado[i] = palabra[i];
        } 
        else 
        {
            int indice = pos - alfabeto;
            resultado[i] = sustitucion[indice];
        }
    }
    resultado[strlen(palabra)] = '\0';
}


// Funcion principal para el módulo 5, que lee el archivo de mensajes cifrados,
// busca una palabra ingresada por el usuario, y muestra en qué mensajes aparece
// mediante el texto original y también cifrada usando cada una de las claves disponibles.
// Con ayuda de los 2 while anidados, lo que hacemos es leer cada línea del archivo,
// extraer el id del mensaje y luego verificar el campo de texto original (campo 5)
// y el campo de texto cifrado (campo 6) para ver si contienen la palabra buscada 
// o su versión cifrada. Si la palabra se encuentra en el texto original, 
// se muestra un mensaje indicando que se encontró en ese mensaje. 
// Si se encuentra en el texto cifrado, se muestra un mensaje 
// indicando que se encontró cifrada con la clave correspondiente. 
// Al final, se muestra el total de apariciones encontradas.
void leer_claves(Clave claves[], int *total_claves) 
{
    FILE *archivo = fopen("Cryptonituv_DB/claves.txt", "r");
    char linea[512];
    
    fgets(linea, sizeof(linea), archivo); // salta encabezado
    
    while (fgets(linea, sizeof(linea), archivo) != NULL) 
    {
        int campo = 0;
        char *token = strtok(linea, "|\n");
        
        while (token != NULL) {
            if (campo == 0) 
            {  
                strcpy(claves[*total_claves].id, token);
            }
            if (campo == 4) 
            { 
                strcpy(claves[*total_claves].sustitucion, token);
            }
            campo++;
            token = strtok(NULL, "|\n");
        }
        (*total_claves)++;
    }
    fclose(archivo);
}

// Función para mostrar las apariciones y equivalencias de una palabra dada un archivo de mensajes cifrados.
// Esto lo que hace es leer el archivo línea por línea, extraer el id del mensaje, 
// y luego verificar el campo de texto original y el campo de texto cifrado para ver 
// si contienen la palabra buscada o su versión cifrada. Si la palabra se encuentra en el 
// texto original, se muestra un mensaje indicando que se encontró en ese mensaje. 
// Si se encuentra en el texto cifrado, se muestra un mensaje indicando que se encontró 
// cifrada con la clave correspondiente. Al final, se muestra el total de apariciones 
// encontradas.
void word_message_analysis_menu(const char *ruta_archivo)
{
    FILE *archivo;
    char linea[2048]; // buffer donde se guarda cada línea
    Clave claves[15];
    int total_claves = 0;
    leer_claves(claves, &total_claves);

    // Pedir palabra al usuario
    char palabra[50];
    printf("Ingresa la palabra a buscar: ");
    scanf("%s", palabra);

    int total_apariciones = 0;
    char id_mensaje[10]; // para guardar el id del mensaje actual

    archivo = fopen(ruta_archivo, "r"); // "r" = solo lectura

    if (archivo == NULL) 
    {
        printf("No se pudo abrir el archivo\n");
        return;
    }

    fgets(linea, sizeof(linea), archivo); // salta el encabezado

    while (fgets(linea, sizeof(linea), archivo) != NULL) 
    {
        int campo = 0;
        char *campo_actual = strtok(linea, "|\n");
        
        while (campo_actual != NULL) {
            if (campo == 0) 
            {
                strcpy(id_mensaje, campo_actual); // Guardar el id del mensaje
            }

            if (campo == 5)
            {
                if (strstr(campo_actual, palabra) != NULL) 
                {
                    printf("Mensaje %s: encontrada en texto original\n", id_mensaje);
                    total_apariciones++;
                }
            }

            if(campo == 6)
            {
                char palabra_cifrada[50];
                char alfabeto[] = "abcdefghijklmnopqrstuvwxyz";
                
                for (int i = 0; i < total_claves; i++) 
                {
                    cifrar_palabra(palabra, alfabeto, claves[i].sustitucion, palabra_cifrada);
                    if (strstr(campo_actual, palabra_cifrada) != NULL) 
                    {
                        printf("Mensaje %s: encontrada cifrada como '%s' con clave %s\n", 
                            id_mensaje, palabra_cifrada, claves[i].id);
                        total_apariciones++;
                    }
                }
            }
            campo++;                        
            campo_actual = strtok(NULL, "|\n");   
        }
    }

    printf("\nTotal de apariciones: %d\n", total_apariciones);
    fclose(archivo);
}

#endif