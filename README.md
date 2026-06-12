# Diseño y Análisis de Algoritmos - Proyecto Final

### Integrantes:
* Juan Pablo Salas Gonzalez
* Vladimir Yepez Contreras

## Descripción breve del sistema
El sistema está compuesto por 5 módulos principales enfocados en el análisis, minería, cifrado y descifrado de mensajes:

1. **Módulo 1: Minería de Mensajes Cifrados**
   * **Función:** Leer mensajes cifrados y listarlos mediante cuatro ordenamientos distintos: prioridad (DESC), fecha (ASC), remitente (ASC) o longitud (ASC).
   * **Archivos:** `mensajes_cifrados.txt` y `remitentes.txt`.
   * **Complejidad:** $O(n \log n)$ y un $O(n + m)$. Cada uno de los cuatro algoritmos utilizados corresponden a Quick Sort, Merge Sort, Heap Sort y Counting Sort.

2. **Módulo 2: Cifrado y Descifrado**
   * **Función:** Cifrar y descifrar mensajes utilizando una tabla de sustitución letra por letra.
   * **Archivos:** `mensajes_cifrados.txt` y `claves.txt`.
   * **Complejidad:** $O(L)$. Se utiliza un mapa de caracteres que remplaza en $O(1)$ para cada elemento de la entrada de longitud $L$.

3. **Módulo 3: Descifrado y Análisis de Frecuencias**
   * **Función:** Analizar frecuencias de letras e inferir equivalencias probables para deducir una palabra objetivo.
   * **Archivos:** `mensajes_cifrados.txt`, `frecuencias.txt` y `palabras_frecuentes.txt`.
   * **Complejidad:** $O(L + k \log k + p)$. $L$ para obtener las frecuencias, $k\log k$ para ordenarlas y $p$ para generar una tabla de sustitución caracter por carcter. 
   * **Nota:** La palabra analizada está cifrada, mientras que la palabra objetivo se obtiene del archivo `palabras_frecuentes.txt` (la cual ya se encuentra descifrada).

4. **Módulo 4: Minería y Análisis de Palabras Top**
   * **Función:** Obtener las 10 palabras presentes en el mayor número de mensajes distintos.
   * **Archivos:** `mensajes_cifrados.txt` y `palabras_frecuentes.txt`.
   * **Complejidad:** $O(T + w \log w)$. Búsqueda en $T$ y ordenamiento en $w \log w$.
   * **Nota:** Si una palabra aparece varias veces dentro de un mismo mensaje, solo se contabilizará una vez para ese mensaje en particular.

5. **Módulo 5: Búsqueda, Análisis y Cifrado**
   * **Función:** Buscar una palabra ingresada por el usuario y mostrar sus apariciones (cantidad y ubicación), junto con sus equivalencias cifradas según cada palabra clave.
   * **Archivos:** `mensajes_cifrados.txt` y `claves.txt`.
   * **Complejidad:** $O(T + q)$. Búsqueda y comparación.
   * **Nota:** Muestra tanto las apariciones cifradas como las no cifradas.

## Organización de archivos
```text
Source_Code/
│
├── Message_Analysis.h              # Módulo 1: Ordenamiento y listado de mensajes
├── Message_Encryption.h            # Módulo 2: Cifrado y descifrado por sustitución
├── Word_Inference.h                # Módulo 3: Análisis de frecuencias e inferencia de palabras
├── Top_Words.h                     # Módulo 4: Top 10 palabras en mensajes distintos
├── Word_Message_Analysis.h         # Módulo 5: Búsqueda de palabras y equivalencias cifradas
├── Menu.c                          # Función main, flujo del menú y llamadas a módulos
│
└── Cryptonituv_DB/                 # Carpeta de base de datos del sistema
    ├── claves.txt                
    ├── frecuencias.txt            
    ├── mensajes.txt               
    ├── mensajes_cifrados.txt      
    ├── palabras_frecuentes.txt    
    └── remitentes.txt             
```

## Instrucciones de compilación
Para clonar el repositorio y compilar el proyecto utilizando GCC, ejecute los siguientes comandos en la terminal:

```bash
# Clonar el repositorio
git clone https://github.com/VladyThrash/DAA_ProyectoFinal.git
cd Source_Code

# Compilar el archivo principal vinculando las librerías necesarias
gcc Menu.c -o programa -lm

# Ejecutar el programa
./programa.exe		 #Windows
./programa		 #Linux
