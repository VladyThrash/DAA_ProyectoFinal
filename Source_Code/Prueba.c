#include<stdio.h>
#include <string.h>

typedef struct {
    char palabra[50];
    int conteo;
} EntradaPalabra;

int buscar_palabra(char arreglo[][50], int total, char *palabra);
void agregar_o_actualizar(EntradaPalabra global[], int *total_global, char *palabra);
void intercambiar(EntradaPalabra *a, EntradaPalabra *b);
void quicksort(EntradaPalabra arr[], int inicio, int fin);

int main()
{
    FILE *archivo;
    char linea[2048]; // buffer donde se guarda cada línea
    EntradaPalabra global[5000];
    int total_global = 0;

    char palabras_mensaje[200][50];
    int total_mensaje = 0;

    archivo = fopen("Cryptonituv_DB/mensajes_cifrados.txt", "r"); // "r" = solo lectura

    if (archivo == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }

    fgets(linea, sizeof(linea), archivo); // salta el encabezado

    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        int campo = 0;
        char *token = strtok(linea, "|\n");
        
        while (token != NULL) {
            if (campo == 5) {
                total_mensaje = 0; // resetear para este mensaje
                
                char *pal = strtok(token, " \n");
                while (pal != NULL) {
                    if (buscar_palabra(palabras_mensaje, total_mensaje, pal) == -1) {
                        strcpy(palabras_mensaje[total_mensaje], pal);
                        total_mensaje++;
                        agregar_o_actualizar(global, &total_global, pal);
                    }
                    pal = strtok(NULL, " \n");
                }
            }
            campo++;                        
            token = strtok(NULL, "|\n");   
        }
    }

    quicksort(global, 0, total_global - 1);

    // Mostrar top 10
    for (int i = 0; i < 10 && i < total_global; i++) {
        printf("%d. %s -> %d mensajes\n", i+1, global[i].palabra, global[i].conteo);
    }
    fclose(archivo);
    return 0;
}

int buscar_palabra(char arreglo[][50], int total, char *palabra) {
    for (int i = 0; i < total; i++) {
        if (strcmp(arreglo[i], palabra) == 0) {
            return i; // la encontró, regresa su posición
        }
    }
    return -1; // no la encontró
}

void agregar_o_actualizar(EntradaPalabra global[], int *total_global, char *palabra) {
    // Buscar si ya existe
    for (int i = 0; i < *total_global; i++) {
        if (strcmp(global[i].palabra, palabra) == 0) {
            global[i].conteo++;  // ya existe, sumar 1
            return;              // terminar la función
        }
    }
    // Si llego aqui, no la encontro entonces agregarla
    strcpy(global[*total_global].palabra, palabra);  
    global[*total_global].conteo = 1;              
    (*total_global)++;
}

void intercambiar(EntradaPalabra *a, EntradaPalabra *b) {
    EntradaPalabra temp = *a;
    *a = *b;
    *b = temp;
}

void quicksort(EntradaPalabra arr[], int inicio, int fin) {
    if (inicio < fin) {
        int pivote = inicio;
        for (int i = inicio + 1; i <= fin; i++) {
            if (arr[i].conteo > arr[pivote].conteo) {
                pivote++;
                intercambiar(&arr[i], &arr[pivote]);
            }
        }
        intercambiar(&arr[inicio], &arr[pivote]);
        quicksort(arr, inicio, pivote - 1);
        quicksort(arr, pivote + 1, fin);
    }
}