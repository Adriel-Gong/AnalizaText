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
