/*
Módulo 4:
    Obtener las 10 palabras presentes en mayor número de mensajes distintos (Minería y análisis):
        *Archivos: mensajes_cifrados.txt  y palabras_frecuentes.txt
        *Complejidad: O(T + w log w)
        *Nota: Si una palabra aparece varias veces en el mismo mensaje, solo contará una vez para ese mensaje.
*/

#ifndef TOP_WORDS_H
#define TOP_WORDS_H

typedef struct 
{
    char palabra[50];
    int conteo;
} EntradaPalabra;

int buscar_palabra(char arreglo[][50], int total, char *palabra);
void agregar_o_actualizar(EntradaPalabra global[], int *total_global, char *palabra);
void intercambiar(EntradaPalabra *a, EntradaPalabra *b);
void quicksort(EntradaPalabra arr[], int inicio, int fin);
void top_words_menu(const char *ruta_archivo);

// Función principal para el módulo 4, que lee el archivo de mensajes cifrados, 
// extrae las palabras de cada mensaje, y cuenta en cuántos mensajes distintos 
// aparece cada palabra. Luego ordena las palabras por su conteo y 
// muestra las 10 más frecuentes.

void top_words_menu(const char *ruta_archivo)
{
    FILE *archivo;
    char linea[2048]; // buffer donde se guarda cada línea
    EntradaPalabra global[5000];
    int total_global = 0;

    char palabras_mensaje[200][50];
    int total_mensaje = 0;

    archivo = fopen(ruta_archivo, "r"); // "r" = solo lectura

    if (archivo == NULL) 
    {
        printf("No se pudo abrir el archivo\n");
        return;
    }

    fgets(linea, sizeof(linea), archivo); // salta el encabezado

    // Aqui lo que hacemos es leer cada línea del archivo, 
    //extraer el campo de palabras (campo 5), y luego para cada palabra en ese campo,
    //verificar si ya la contamos para este mensaje. Si no, la agregamos a
    //palabras_mensaje y luego actualizamos el conteo global usando agregar_o_actualizar.
    while (fgets(linea, sizeof(linea), archivo) != NULL) 
    {
        int campo = 0;
        char *campo_actual = strtok(linea, "|\n");
        
        while (campo_actual != NULL) 
        {
            if (campo == 5) 
            {
                total_mensaje = 0; // resetear para este mensaje
                
                char *palabra_actual = strtok(campo_actual, " \n");
                while (palabra_actual != NULL) 
                {
                    if (buscar_palabra(palabras_mensaje, total_mensaje, palabra_actual) == -1) 
                    {
                        strcpy(palabras_mensaje[total_mensaje], palabra_actual);
                        total_mensaje++;
                        agregar_o_actualizar(global, &total_global, palabra_actual);
                    }
                    palabra_actual = strtok(NULL, " \n");
                }
            }
            campo++;                        
            campo_actual = strtok(NULL, "|\n");   
        }
    }

    quicksort(global, 0, total_global - 1);

    // Mostrar top 10
    for (int i = 0; i < 10 && i < total_global; i++) 
    {
        printf("%d. %s -> %d mensajes\n", i+1, global[i].palabra, global[i].conteo);
    }
    fclose(archivo);
    return;
}

// Funcion auxiliar para buscar una palabra en un arreglo de palabras
int buscar_palabra(char arreglo[][50], int total, char *palabra) 
{
    for (int i = 0; i < total; i++) 
    {
        if (strcmp(arreglo[i], palabra) == 0) {
            return i; // la encontró, regresa su posición
        }
    }
    return -1; // no la encontró
}

// Función auxiliar para agregar una palabra al conteo global o actualizar 
//su conteo si ya existe
void agregar_o_actualizar(EntradaPalabra global[], int *total_global, char *palabra) 
{
    // Buscar si ya existe
    for (int i = 0; i < *total_global; i++) 
    {
        if (strcmp(global[i].palabra, palabra) == 0) 
        {
            global[i].conteo++;  // ya existe, sumar 1
            return;              // terminar la función
        }
    }
    // Si llego aqui, no la encontro entonces agregarla
    strcpy(global[*total_global].palabra, palabra);  
    global[*total_global].conteo = 1;              
    (*total_global)++;
}

// Funciones para ordenar el arreglo global de palabras por su conteo usando quicksort
void intercambiar(EntradaPalabra *a, EntradaPalabra *b) 
{
    EntradaPalabra temp = *a;
    *a = *b;
    *b = temp;
}

// Ordenar de mayor a menor conteo usando quicksort 
void quicksort(EntradaPalabra arr[], int inicio, int fin) 
{
    if (inicio < fin) 
    {
        int pivote = inicio;
        for (int i = inicio + 1; i <= fin; i++) 
        {
            if (arr[i].conteo > arr[pivote].conteo) 
            {
                pivote++;
                intercambiar(&arr[i], &arr[pivote]);
            }
        }
        intercambiar(&arr[inicio], &arr[pivote]);
        quicksort(arr, inicio, pivote - 1);
        quicksort(arr, pivote + 1, fin);
    }
}

#endif