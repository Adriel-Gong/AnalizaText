/*
Nombre del programa: AnalizaText
Objetivo: Programa analizador de archivos de texto .txt que retorna
conteo de caracteres y palabras, palabras mas usadas (frecuencia de uso y porcentaje) 
en forma de graficas a traves de un nuevo archivo .txt
Autores: Ek Gongora Adriel, De la Rosa Garcia Oscar, Novelo Hernandez Jarib Alberto
Fecha: 12/05/25
Version: 1.0
*/
#define MAX_PATH 256
#define MAX_CONTENT 100000
#define MAX_WORDS 5000
#define MAX_WORD_LEN 50

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Variables globales
char file_content[MAX_CONTENT];
long file_size = 0;
int total_words = 0;

struct WordInfo {
	char word[MAX_WORD_LEN];
	int count;
} word_freq[MAX_WORDS];

int unique_words = 0;

// Funcion para identificar caracteres
int is_letter(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

//Funcion para convertir mayusculas a minusculas para que tengan el mismo valor ASCII
char to_lower(char c) {
	if (c >= 'A' && c <= 'Z') return c + 32;
	return c;
}

//Funcion ordenamiento de frecuencias de mayor a menor
void sort_words() {
	int i;
	
	for (i = 0; i < unique_words - 1; i++) {
		for (int j = i + 1; j < unique_words; j++) {
			if (word_freq[j].count > word_freq[i].count) {
				struct WordInfo temp = word_freq[i];
				word_freq[i] = word_freq[j];
				word_freq[j] = temp;
			}
		}
	}
}

//Funcion para analizar el texto y contar la frecuencia de las palabras
void process_text() {
	total_words = 0; 
	unique_words = 0; 
	char current_word[MAX_WORD_LEN] = {0}; //Vector temporal para almacenar la palabra actual
	int word_pos = 0; 
	int i, j; 
	
	//Bucle para cada caracter en el texto cargado
	for (i = 0; i < file_size; i++) {
		//If para detectar el inicio y fin de una palabra y a su vez, convertirla toda en minuscula
		if (is_letter(file_content[i])) {
			if (word_pos < MAX_WORD_LEN - 1) {
				current_word[word_pos++] = to_lower(file_content[i]);
			}
		} else if (word_pos > 0) {
			current_word[word_pos] = '\0'; //Detecta fin de la palabra
			total_words++; //Incrementa el numero de palabras
			
			//Buscar palabras repetidas y aumentar su frecuencia de ser asi
			int found = 0;
			for (j = 0; j < unique_words; j++) {
				if (strcmp(word_freq[j].word, current_word) == 0) {
					word_freq[j].count++;
					found = 1;
					break;
				}
			}
			
			//Registrar palabras nuevas
			if (!found && unique_words < MAX_WORDS) {
				strcpy(word_freq[unique_words].word, current_word);
				word_freq[unique_words].count = 1;
				unique_words++;
			}
			word_pos = 0;
		}
	}
	sort_words();
}

//Funcion encargada a la carga del archivo de texto 
void text_reader() {
	char file_path[MAX_PATH]; 
	FILE *file = NULL; //Puntero al archivo
	int c; 
	
	printf("\nIngrese la ruta del archivo .txt: ");
	scanf("%255s", file_path);
	
	//Verificar extension .txt
	if (strstr(file_path, ".txt") == NULL) {
		printf("Error: El archivo debe tener extension .txt\n");
		return;
	}
	
	//Abrir el archivo de texto
	file = fopen(file_path, "r");
	if (file == NULL) {
		printf("Error al cargar el archivo, verifique la ruta e intente de nuevo.\n");
		return;
	}
	
	//Leer contenido del archivo
	file_size = 0;
	while (file_size < MAX_CONTENT - 1 && (c = fgetc(file)) != EOF) {
		file_content[file_size++] = (char)c;
	}
	file_content[file_size] = '\0';
	fclose(file);
	
	//Verificar si el archivo esta vacio
	if (file_size == 0) {
		printf("Error: El archivo esta vacio.\n");
		return;
	}
	
	process_text();
	printf("\nArchivo cargado correctamente.\n");
}

//Funcion encargada para el analisis y muestreo de la informacion en numeros
void text_stadistics() {
	if (file_size == 0) {
		printf("\nError: No hay archivo cargado. Use la opcion 1 primero.\n");
		return;
	}
	
	// Calcular el promedio de caracteres por palabra
	int total_chars_in_words = 0;
	for (int i = 0; i < unique_words; i++) {
		total_chars_in_words += strlen(word_freq[i].word) * word_freq[i].count;
	}
	float avg_chars_per_word = (float)total_chars_in_words / total_words;
	
	printf("\n=== ESTADISTICAS DEL TEXTO ===\n");
	printf("Caracteres totales: %ld\n", file_size);
	printf("Palabras totales: %d\n", total_words);
	printf("Promedio de caracteres por palabra: %.2f\n", avg_chars_per_word);
	
	// Mostrar palabras mas frecuentes
	printf("\nTop 5 palabras mas frecuentes:\n");
	int limit = (unique_words < 5) ? unique_words : 5;
	for (int i = 0; i < limit; i++) {
		float percentage = (float)word_freq[i].count / total_words * 100;
		printf("%d. %s - %d ocurrencias (%.2f%%)\n", 
			   i+1, word_freq[i].word, word_freq[i].count, percentage);
	}
}

// Funcion encargada para la creacion y muestreo de graficas simples en la consola
void text_graphs() {
	int limit, len, max_len = 0, min_len = MAX_WORD_LEN, total_chars = 0;
	int i, j;
	float percentage, avg_len;
	char longest_word[MAX_WORD_LEN] = "";
	char shortest_word[MAX_WORD_LEN] = "";
	
	// Verificar si hay archivo cargado
	if (file_size == 0) {
		printf("\nError: No hay archivo cargado. Use la opcion 1 primero.\n");
		return;
	}
	
	printf("\n=== GRAFICAS DE FRECUENCIA ===\n");
	
	// Seleccionar top 5 palabras más usadas o reducir el top si hay menos de 5 palabras únicas
	if (unique_words < 5) {
		limit = unique_words;
	} else {
		limit = 5;
	}
	
	// Calcular porcentaje de frecuencia para las palabras más usadas
	printf("\nTop %d palabras más frecuentes:\n", limit);
	for (i = 0; i < limit; i++) {
		percentage = (float)word_freq[i].count / total_words * 100;
		printf("\n%s (%.2f%%): ", word_freq[i].word, percentage);
		for (j = 0; j < word_freq[i].count; j++) {
			printf("*");
		}
	}
	printf("\n");
	
	// Gráfico de comparación de longitudes
	printf("\nCOMPARACION DE LONGITUDES DE PALABRAS:\n");
	
	//bug fix de archivo vacio
	if (unique_words == 0) return;
	
	// Encontrar palabra más larga, más corta y promedio
	for (i = 0; i < unique_words; i++) {
		len = strlen(word_freq[i].word);
		total_chars += len;
		
		if (len > max_len) {
			max_len = len;
			strcpy(longest_word, word_freq[i].word);
		}
		if (len < min_len) {
			min_len = len;
			strcpy(shortest_word, word_freq[i].word);
		}
	}
	
	avg_len = (float)total_chars / unique_words;
	
	//Salida
	printf("\nPalabra más larga (%d caracteres): %s\n", max_len, longest_word);
	printf("Promedio de caracteres por palabra: %.2f\n", avg_len);
	printf("Palabra más corta (%d caracteres): %s\n", min_len, shortest_word);
	
	// Gráfico de comparación y frecuencias en asteriscos
	printf("\nGrafico de comparacion:\n");
	printf("Palabra más larga [%s]: ", longest_word);
	for (i = 0; i < max_len; i++) printf("*");
	printf("\nPromedio de caracteres:    ");
	for (i = 0; i < (int)avg_len; i++) printf("*");
	printf("\nPalabra más corta [%s]: ", shortest_word);
	for (i = 0; i < min_len; i++) printf("*");
	printf("\n");
}

//Funcion encargada de crear un archivo de texto y almacenar los datos y graficas del texto
void text_data_file() {
	//Declaracion de variables agrupadas
	char output_path[MAX_PATH], longest_word[MAX_WORD_LEN] = "", shortest_word[MAX_WORD_LEN] = "";
	FILE *output_file;
	int total_chars_in_words = 0, limit, max_len = 0, min_len = MAX_WORD_LEN;
	int total_chars = 0, i, j, len;
	float avg_chars_per_word, percentage, avg_len;
	
	//Verificar si hay archivo cargado
	if (file_size == 0) {
		printf("\nError: No hay archivo cargado. Use la opcion 1 primero.\n");
		return;
	}
	
	//Obtener nombre del archivo de salida
	printf("\nIngrese el nombre del archivo para los resultados: ");
	scanf("%255s", output_path);
	while(getchar() != '\n');
	
	//Añadir extensión .txt si no la tiene
	if (strstr(output_path, ".txt") == NULL) {
		strcat(output_path, ".txt");
	}
	
	//Crear archivo de salida
	output_file = fopen(output_path, "w");
	if (output_file == NULL) {
		printf("Error al crear el archivo de resultados.\n");
		return;
	}
	
	//Calcular estadísticas básicas
	for (i = 0; i < unique_words; i++) {
		total_chars_in_words += strlen(word_freq[i].word) * word_freq[i].count;
	}
	avg_chars_per_word = (float)total_chars_in_words / total_words;
	
	// Escribir estadísticas básicas
	fprintf(output_file, "=== ANALISIS DE TEXTO ===\n\n");
	fprintf(output_file, "ESTADISTICAS BASICAS:\n");
	fprintf(output_file, "Caracteres totales: %ld\n", file_size);
	fprintf(output_file, "Palabras totales: %d\n", total_words);
	fprintf(output_file, "Promedio de caracteres por palabra: %.2f\n\n", avg_chars_per_word);
	
	// Top 5 palabras más frecuentes
	fprintf(output_file, "TOP 5 PALABRAS MAS FRECUENTES:\n");
	limit = (unique_words < 5) ? unique_words : 5;
	for (i = 0; i < limit; i++) {
		percentage = (float)word_freq[i].count / total_words * 100;
		fprintf(output_file, "%d. %s - %4d ocurrencias (%.2f%%)\n", 
				i+1, word_freq[i].word, word_freq[i].count, percentage);
	}
	fprintf(output_file, "\n");
	
	// Gráficas de frecuencia
	fprintf(output_file, "GRAFICAS DE FRECUENCIA:\n\n");
	for (i = 0; i < limit; i++) {
		percentage = (float)word_freq[i].count / total_words * 100;
		fprintf(output_file, "%s (.2f%%): ", word_freq[i].word, percentage);
		for (j = 0; j < word_freq[i].count; j++) {
			fprintf(output_file, "*");
		}
		fprintf(output_file, "\n");
	}
	fprintf(output_file, "\n");
	
	// Comparación de longitudes de palabras
	fprintf(output_file, "COMPARACION DE LONGITUDES DE PALABRAS:\n\n");
	for (i = 0; i < unique_words; i++) {
		len = strlen(word_freq[i].word);
		total_chars += len;
		if (len > max_len) {
			max_len = len;
			strcpy(longest_word, word_freq[i].word);
		}
		if (len < min_len) {
			min_len = len;
			strcpy(shortest_word, word_freq[i].word);
		}
	}
	avg_len = (float)total_chars / unique_words;
	
	fprintf(output_file, "Palabra mas larga (%d caracteres): %s\n", max_len, longest_word);
	fprintf(output_file, "Promedio de caracteres por palabra: %.2f\n", avg_len);
	fprintf(output_file, "Palabra mas corta (%d caracteres): %s\n\n", min_len, shortest_word);
	
	fprintf(output_file, "Comparacion:\n");
	fprintf(output_file, "Larga [%s]: ", longest_word);
	for (i = 0; i < max_len; i++) fprintf(output_file, "*");
	fprintf(output_file, "\nPromedio:    ");
	for (i = 0; i < (int)avg_len; i++) fprintf(output_file, "*");
	fprintf(output_file, "\nCorta [%s]: ", shortest_word);
	for (i = 0; i < min_len; i++) fprintf(output_file, "*");
	fprintf(output_file, "\n");
	
	// Cerrar archivo
	fclose(output_file);
	printf("\nResultados guardados exitosamente en: %s\n", output_path);
}

// Función principal
int main() {
	int option;
	int file_loaded = 0;
	
	printf("============================\n");
	printf("ANALIZADOR DE TEXTOS .TXT\n");
	printf("============================\n");
	
	do {
		printf("\n1. Cargar archivo de texto\n");
		printf("2. Mostrar estadisticas\n");
		printf("3. Mostrar graficas\n");
		printf("4. Guardar resultados\n");
		printf("5. Salir\n");
		printf("Seleccione una opcion: ");
		scanf("%d", &option);
		while(getchar() != '\n'); //bug fix bucle infinito
		
		switch(option) {
		case 1: 
			text_reader();
			file_loaded = (file_size > 0);
			break;
		case 2: 
			if (file_loaded) text_stadistics();
			else printf("\nError: Primero cargue un archivo (opcion 1)\n");
			break;
		case 3: 
			if (file_loaded) text_graphs();
			else printf("\nError: Primero cargue un archivo (opcion 1)\n");
			break;
		case 4: 
			if (file_loaded) text_data_file();
			else printf("\nError: Primero cargue un archivo (opcion 1)\n");
			break;
		case 5: 
			printf("\nSaliendo del programa...\n");
			return 0;
		default: 
			printf("\nOpcion no valida, intente de nuevo.\n");
		}
	} while(option != 5);
}
