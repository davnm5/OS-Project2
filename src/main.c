/* Author : David Neira Martrus
   Date: 03/02/2020
   Project 2
   Teacher: Ángel López Aguirre
*/

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "../include/lru.h"
#include "../include/optimal.h"
#include "../include/fifo.h"

#define MAX 500
#define GB 30
#define MB 20
#define KB 10

int mem_ref[100] = {0};
int pas_int, las_int, page_size_int, process_size_int, page_frame_int;
int contador = 0;
char buffer[100], string[MAX] = "";
char *las_v, *pas_v, *page_size_v, *process_name_v, *process_size_v, *algorithm_v="", *memref_v;
char las[MAX], pas[MAX], page_size[MAX], process_size[MAX], process_name[MAX], algorithm[MAX], comentario[MAX], memref[MAX], page_frame[MAX];
char *prueba, *lista[MAX];
char *tokenizar(char linea[], char *token);
int obtener_numero(char *valor);
void leer_configuracion(char *file);
char *removerNumeros(char *input);
void removeSubstr(char *string, char *sub);
void calcular();
void crear_string();

void leer_configuracion(char *name)         /*Se lee la configuracion del archivo*/
{
    FILE *archivo;
    archivo = fopen(name, "r");
    fgets(comentario, MAX, archivo);
    fgets(las, MAX, archivo);
    las_v = tokenizar(las, "=");
    las_int = obtener_numero(las_v);
    fgets(pas, MAX, archivo);
    pas_v = tokenizar(pas, "=");
    pas_int = obtener_numero(pas_v);
    fgets(page_size, MAX, archivo);
    page_size_v = tokenizar(page_size, "=");
    page_size_int = obtener_numero(page_size_v);
    fgets(comentario, MAX, archivo);
    fgets(process_name, MAX, archivo);
    process_name_v = tokenizar(process_name, "=");
    fgets(process_size, MAX, archivo);
    process_size_v = tokenizar(process_size, "=");
    process_size_int = obtener_numero(process_size_v);
    fgets(memref, MAX, archivo);
    memref_v = tokenizar(memref, "=");
    fgets(algorithm, MAX, archivo);
    algorithm_v = tokenizar(algorithm, "=");
    fgets(page_frame, MAX, archivo);
    page_frame_int = atoi(tokenizar(page_frame, "="));

    fclose(archivo);
}

char *tokenizar(char linea[], char *token)      /*Se separan las lineas del archivo por el token recibido*/
{
    strtok(linea,"\n");
    char *cadena = strtok(linea, token);
    if (cadena != NULL)
    {
        cadena = strtok(NULL, token);
        return cadena;
    }
}

int obtener_numero(char *valor)    /*Función que devuelve la parte numérica de la cadena*/
{
    char aux[MAX];
    int aux_int = 0;
    strcpy(aux, valor);

    int contador = 0;
    int x = 0;
    removeSubstr(aux, removerNumeros(valor));
    aux_int = atol(aux);

    return aux_int;
}

int buscar_potencia(int numero, char *valor)   /*función que devuelve la potencia adecuada y además realiza la conversión de unidades*/
{
    int contador = 0;
    char aux[MAX];
    strcpy(aux, valor);

    char *r = removerNumeros(valor);
    r[2] = '\0';

    while (pow(2, contador) < numero)
    {
        contador++;
    }

    if (strcmp(r, "GB") == 0)
        contador = contador + GB;
    if (strcmp(r, "MB") == 0)
        contador = contador + MB;
    if (strcmp(r, "KB") == 0)
        contador = contador + KB;

    return contador;
}

char *removerNumeros(char *input)
{
    char *dest = input;
    char *src = input;

    while (*src)
    {
        if (isdigit(*src))
        {
            src++;
            continue;
        }
        *dest++ = *src++;
    }
    *dest = '\0';
    return input;
}

void removeSubstr(char *string, char *sub)
{
    char *match;
    int len = strlen(sub);
    while ((match = strstr(string, sub)))
    {
        *match = '\0';
        strcat(string, match + len);
    }
}

void calcular()
{
    int valid;

    printf("**Logical Address**\n");
    printf("     Page number: --> %d bits\n", buscar_potencia(las_int, las_v) - buscar_potencia(page_size_int, page_size_v));
    printf("          Offset: --> %d bits\n", buscar_potencia(page_size_int, page_size_v));
    printf("             PTE: --> %.0f pages\n\n", pow(2, buscar_potencia(las_int, las_v) - buscar_potencia(page_size_int, page_size_v)));
    printf("**Physical Address:**\n");
    printf("          Frames: --> %.0f frames\n", pow(2, buscar_potencia(pas_int, pas_v) - buscar_potencia(page_size_int, page_size_v)));
    printf(" Page Table Size: --> %.0f entries\n", pow(2, buscar_potencia(process_size_int, process_size_v) - buscar_potencia(page_size_int, page_size_v)));
    valid = process_size_int / page_size_int;

    if (valid % 2 != 0)
    {
        valid = valid + 1;
    }
    printf("     Pages valid: --> %d entries\n", valid);
    crear_string();
    printf("Reference string: --> %s\n", string);
    printf("       Algorithm: --> %s\n\n", algorithm_v);
}

void crear_string()
{
    char *cadena;
    cadena = strtok(memref_v, ",");
    while (cadena != NULL)
    {
        lista[contador] = cadena;
        contador++;
        cadena = strtok(NULL, ",");
    }

    char t[2] = ",";
    char buf[10];
    for (int i = 0; i < contador; i++)
    {
        int r = atoi(lista[i]) / page_size_int;
        snprintf(buf, sizeof(buf), "%d", r);
        strcat(string, buf);
        if (i < contador - 1)
            strcat(string, t);
    }
}

int main(int argc, char *argv[])
{
    int list_int[MAX];
    if (argc == 2)
    {
        leer_configuracion(argv[1]);
        calcular();

        for (int i = 0; i < contador; i++)
        {
            int x = atoi(lista[i]);
            list_int[i] = x;
        }
        algorithm_v[strlen(algorithm_v)-1]='\0';
        if (strcmp(algorithm_v, "LRU") == 0)
            execute_lru(page_frame_int, contador, list_int);
        if (strcmp(algorithm_v, "FIFO") == 0)
            execute_fifo(page_frame_int, contador, list_int);
        if (strcmp(algorithm_v, "OPTIMAL") == 0)
            execute_optimal(page_frame_int, contador, list_int);
    }
    else
    {
        printf("Uso:: ./prs [ARCHIVO_CFG]\n");
    }

    return 0;
}
