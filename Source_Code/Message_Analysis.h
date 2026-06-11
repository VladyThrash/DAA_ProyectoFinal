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

//Struct para vaciar la información del archivo.
struct mensaje{
	char id[20];
    char remitente[50];
    char prioridad[10];
    char fecha[20];
    char clave_id[20];
    char texto_original[256];
    char texto_cifrado[256];
};

//Path al archivo txt.
char path[] = "Cryptonituv_DB/mensajes_cifrados.txt";

//Puntero para crear arreglo de structs
struct mensaje *mensajes = NULL;

//Toma el control del flujo del programa desde main.
void message_analysis_menu();

//Inicializa la lectura del archivo txt y extrae la información en un struct.
int extraer_txt();

//Imprimir todos los registros obtenidos en el arreglo de structs.
void imprimir_registros(int n);

//Ordena en DESC los mensajes basados en prioridad.
//A cada categoria: alta, media y baja, se le asigna un peso 0, 1 y 2 respectivamente. El peso se vuelve el criterio de conteo.
//Complejidad: O(n + k), donde k es el número de categorias.
void counting_sort(struct mensaje *arr, int size);
int obtener_peso(char *prioridad);

//Ordena en ASC los mensajes basado en las fechas  formato ISO: AAAA-MM-DD.
//Aplica strcmp para obtener directamente el valor lexicografico.
//Complejidad: O(n*log(n)). Requiere generar memoria de manera dinamica.
void merge_sort(struct mensaje *arr, int izq, int der);
void merge(struct mensaje *arr, int izq, int mid, int der);

//Ordena en ASC los mensajes badaso en el nombre del remitente.
//Aplica strcmp para obtener directamente el valor lexicografico.
//Complejidad: O(n*log(n)) para caso promedio. En peor caso puede ser O(n^2).
void quick_sort(struct mensaje *arr, int izq, int der);
int partition(struct mensaje *arr, int izq, int der); //Aplica partición Hoare.

//Ordena en DESC los mensajes basado en la longitud del mensaje cifrado.
//Aplica strlen para obtener directamente el número de caracteres de la cadena.
//Complejidad: O(2n*log(n)).
void heap_sort(struct mensaje *arr, int size);
void push(struct mensaje *h, int *n, struct mensaje dato);
struct mensaje pop(struct mensaje *h, int *n);

//Para copiar el indice de un arreglo de structs en otro.
void copy(struct mensaje *dst, struct mensaje *src);

//Para hacer swap entre indices del arreglo de structs.
void swap(struct mensaje *msg1, struct mensaje *msg2);


void message_analysis_menu(){
	int num_registros = extraer_txt();
    if(num_registros == 0){ //Validar que se extrajo correctamente la información del txt.
		return;
	}

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
                counting_sort(mensajes, num_registros); //Ordens prioridad DESC.
                printf("\n--- LISTADO EN PRIORIDAD ORDEN DESC ---\n");
                imprimir_registros(num_registros);
                break;

            case 2:
                merge_sort(mensajes, 0, num_registros - 1); //Ordenar por fecha ASC.
                printf("\n--- LISTADO EN FECHA ORDEN ASC ---\n");
                imprimir_registros(num_registros);
                break;

            case 3:
                quick_sort(mensajes, 0, num_registros - 1); //Ordenar por remitente ASC.
                printf("\n--- LISTADO EN REMITENTE ORDEN ASC ---\n");
                imprimir_registros(num_registros);
                break;

            case 4:
                heap_sort(mensajes, num_registros); //Ordenar por longitud del mensaje ASC.
                printf("\n--- LISTADO EN LONGITUD MENSAJE ORDEN ASC ---\n");
                imprimir_registros(num_registros);
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
	free(mensajes); //Liberar memoria del arreglo de structs.
}

int extraer_txt(){
	//Abrimos el archivo en modo lectura ("r").
    FILE *archivo = fopen(path, "r");
    if (archivo == NULL) {
        printf("ERROR: NO SE PUDO ABRIR EL ARCHIVO...\n");
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
        printf("EL ARCHIVO NO CONTIENE DATOS...\n");
        fclose(archivo);
        return 0;
    }

	//Crear el arreglo con el número total de registros.
    mensajes = (struct mensaje*)malloc(sizeof(struct mensaje)*total_lineas);
    if(mensajes == NULL){
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
			strcpy(mensajes[i].id, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mensajes[i].remitente, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mensajes[i].prioridad, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mensajes[i].fecha, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mensajes[i].clave_id, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mensajes[i].texto_original, token);
		}

        token = strtok(NULL, "|\n");
        if (token != NULL) {
			strcpy(mensajes[i].texto_cifrado, token);
		}
	}

	fclose(archivo); //Cerrar el archivo.
	return total_lineas;
}

void imprimir_registros(int n){
	printf("ID\t|\tRemitente\t|\tPrioridad\t|\tFecha\t|\tClave ID\t|\tTexto Cifrado\n");
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
	
	for(int i = 0; i < n; i++){
		printf("%s\t|\t%s\t\t|\t%s\t|\t%s\t|\t%s\t|\t%s\n", mensajes[i].id, mensajes[i].remitente, mensajes[i].prioridad,
													mensajes[i].fecha, mensajes[i].clave_id, mensajes[i].texto_cifrado);
	}
	printf("-----------------------------------------------------------------------------------------------------------------------------\n");
}

void counting_sort(struct mensaje *arr, int size){
    //Arreglo temporal para guardar el resultado ordenado.
    struct mensaje *output = (struct mensaje*)malloc(sizeof(struct mensaje) * size);
    int *count = (int*)calloc(3, sizeof(int)); //Arreglo de conteo.

    //Contar mensajes de cada categoria.
    for(int i = 0; i < size; i++){
        int peso = obtener_peso(arr[i].prioridad);
        count[peso]++;
    }

    //Calcular las posiciones acumuladas.
    for(int i = 1; i < 3; i++){
        count[i] += count[i - 1];
    }

    //Construir el arreglo de salida, recorremos el arreglo original de atrás hacia adelante.
    for(int i = size - 1; i >= 0; i--){
        int peso = obtener_peso(arr[i].prioridad);
        int i_destino = count[peso] - 1; //Restamos 1 al conteo para obtener el índice.
        copy(&output[i_destino], &arr[i]); //Copiamos en su indice destino.
        count[peso]--; //decrementamos el conteo.
    }

    //Reescribir el arreglo original.
    for(int i = 0; i < size; i++){
        copy(&arr[i], &output[i]);
    }
    free(count);
    free(output);
}

int obtener_peso(char *prioridad){
    if(strcmp(prioridad, "baja") == 0){ //Prioridades invertidas para orden DESC.
        return 2;
    }
    if(strcmp(prioridad, "media") == 0){
        return 1;
    }
    if(strcmp(prioridad, "alta") == 0){
        return 0;
    }
    return 0;
}

void merge_sort(struct mensaje *arr, int izq, int der){
    if(izq < der){
        int mid = (izq + der)/2;
        merge_sort(arr, izq, mid); //sub_arr izquierdo
        merge_sort(arr, mid+1, der); //sub_arr derecho
        merge(arr, izq, mid, der);
    }
}

void merge(struct mensaje *arr, int izq, int mid, int der){
    int tam = der - izq + 1; //Tamaño del subarreglo mezclado.
    struct mensaje *aux = (struct mensaje*)malloc(sizeof(struct mensaje)*tam); //Arreglo auxiliar.
    int i_der = mid + 1; //Para iterar sobre el subarreglo derecho.
    int izq_aux = izq; //Para iterar sobre el subarreglo izquierdo.
    for(int i = 0; i < tam; i++){
        if(izq > mid){ //Ya iteramos todo el subarreglo izquierdo, vaciamos lo que queda del derecho.
            copy(&aux[i], &arr[i_der]);
            i_der++;
            continue;
        }
        if(i_der > der){ //Ya iteramos todo el subarreglo derecho, vaciamos lo que queda del izquierdo.
            copy(&aux[i], &arr[izq]);
            izq++;
            continue;
        }
        //strcmp(cad1, cad2): devuelve 0 si cad1 == cad2, negativo si cad1 < cad2, positivo si cad1 > cad2.
        if(strcmp(arr[izq].fecha, arr[i_der].fecha) < 0){
            copy(&aux[i], &arr[izq]); //El valor en el subarreglo izquierdo es menor, copiamos en aux.
            izq++;
        }
        else{
            copy(&aux[i], &arr[i_der]); //El valor en el subarreglo derecho es menor, copiamos en aux.
            i_der++;
        }
    }
    for(int i = 0; i < tam; i++, izq_aux++){
        copy(&arr[izq_aux], &aux[i]); //Vaciamos aux ordenado en el subarreglo mezclado.
    }
    free(aux);
}

void quick_sort(struct mensaje *arr, int izq, int der){
    if(izq>=der){
        return;
    }
        
    int p = partition(arr, izq, der); //Aplica partición Hoare.
    quick_sort(arr, izq, p - 1); //Subarreglo izquierdo.
    quick_sort(arr, p + 1, der); //Subarreglo derecho.
}

int partition(struct mensaje *arr, int izq, int der){
    int i = izq, j = der-1; //Para iterar en ambos sentidos.
    struct mensaje *pivote = &arr[der]; //último
    
    //strcmp(cad1, cad2): devuelve 0 si cad1 == cad2, negativo si cad1 < cad2, positivo si cad1 > cad2.
    while(i <= j){
        while(strcmp(arr[i].remitente, pivote->remitente) < 0){ //aqui no hay que poner i <=right, porque la condicion se rompe al compararse con el pivote
            i++; //Encontramos el indice del primer elemento mayor a pivote.
        }
        while(j>=izq && strcmp(arr[j].remitente, pivote->remitente) > 0){
            j--; //Encontramos el indice del primer elemento menor a pivote.
        }
        if(i <= j){        
            swap(&arr[i], &arr[j]); //Ambos se ordenan con respecto al pivote: menor -> subarreglo izquierdo, mayor -> derecho.
            i++; 
            j--;
        }
    }
    
    swap(&arr[j + 1], &arr[der]); //Colocamos el pivote en el limite de los subarreglos.
    return j+1; //Retornamos el indice del pivote.
}

void heap_sort(struct mensaje *arr, int size){
    struct mensaje *aux = (struct mensaje*)malloc(sizeof(struct mensaje)*size);
    int n = 0;

    for(int i=0; i<size; i++){
        push(aux, &n, arr[i]);
    }
  
    for(int i=0; i<size; i++){ 
        struct mensaje out = pop(aux, &n); 
        copy(&arr[i], &out);  
    }

    free(aux);
}

void push(struct mensaje *h, int *n, struct mensaje dato){
    //Insertar al final del arreglo.
    int i = *n;
    h[i] = dato;
    *n = *n + 1; //Incrementamos el contador de elementos.
  
    //Flotar
    while(i > 0) {
        int padre = (i - 1) / 2;  
        //Comparamos usando la longitud del mensaje con strlen.
        //Si el hijo (i) es menor alfabéticamente que el padre, flotamos.
        if(strlen(h[i].texto_cifrado) < strlen(h[padre].texto_cifrado)){
            swap(&h[i], &h[padre]);
            i = padre; //Evaluamos desde la nueva posición.
        } 
        else{
            break; //Si ya es mayor o igual, terminó de flotar.
        }    
    } 
}

struct mensaje pop(struct mensaje *h, int *n){
    //Es la raiz a retornar.
    struct mensaje temp = h[0];
    
    //Hacer que el último elemento sea el primero.
    *n = *n - 1;
    h[0] = h[*n]; 
  
    //Hundirlo.
    int i = 0;
    while (2 * i + 1 < *n) {
        int hijo_izq = 2 * i + 1;
        int hijo_der = 2 * i + 2;
        int hMenor = hijo_izq;

        //Si el hijo derecho existe y es menor que el izquierdo, actualizamos hMenor.
        if(hijo_der < *n && strlen(h[hijo_der].texto_cifrado) < strlen(h[hijo_izq].texto_cifrado)){
            hMenor = hijo_der;
        }
        
        //Si el padre es mayor que el menor de los hijos, lo hundimos.
        if(strlen(h[i].texto_cifrado) > strlen(h[hMenor].texto_cifrado)){
            swap(&h[i], &h[hMenor]);
            i = hMenor; //Ahora el padre baja a la posición del hijo.
        } 
        else{
            break; //Ya encontró su lugar correcto.
        }
    }
    return temp;
}

void copy(struct mensaje *dst, struct mensaje *src){
    //Copiamos el contenido de src en dst.
    strcpy(dst->id, src->id);
    strcpy(dst->remitente, src->remitente);
    strcpy(dst->prioridad, src->prioridad);
    strcpy(dst->fecha, src->fecha);
    strcpy(dst->clave_id, src->clave_id);
    strcpy(dst->texto_original, src->texto_original);
    strcpy(dst->texto_cifrado, src->texto_cifrado);
}

void swap(struct mensaje *msg1, struct mensaje *msg2){
    struct mensaje aux;

    //Copiamos contenido de msg1 a aux.
    strcpy(aux.id, msg1->id);
    strcpy(aux.remitente, msg1->remitente);
    strcpy(aux.prioridad, msg1->prioridad);
    strcpy(aux.fecha, msg1->fecha);
    strcpy(aux.clave_id, msg1->clave_id);
    strcpy(aux.texto_original, msg1->texto_original);
    strcpy(aux.texto_cifrado, msg1->texto_cifrado);

    //Copiamos contenido de msg2 a msg1.
    strcpy(msg1->id, msg2->id);
    strcpy(msg1->remitente, msg2->remitente);
    strcpy(msg1->prioridad, msg2->prioridad);
    strcpy(msg1->fecha, msg2->fecha);
    strcpy(msg1->clave_id, msg2->clave_id);
    strcpy(msg1->texto_original, msg2->texto_original);
    strcpy(msg1->texto_cifrado, msg2->texto_cifrado);

    //Copiamos contenido de aux a msg2.
    strcpy(msg2->id, aux.id);
    strcpy(msg2->remitente, aux.remitente);
    strcpy(msg2->prioridad, aux.prioridad);
    strcpy(msg2->fecha, aux.fecha);
    strcpy(msg2->clave_id, aux.clave_id);
    strcpy(msg2->texto_original, aux.texto_original);
    strcpy(msg2->texto_cifrado, aux.texto_cifrado);
}

#endif