#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/list.h"
#define MAX 500

char linea1[MAX], linea2[MAX], linea3[MAX], linea4[MAX], linea5[MAX];
char *insert_values, *delete_values, *search_values, *update_values_old, *update_values_new;
int *list_insert, *list_update, *list_delete, *list_search, *list_update_old, *list_update_new, lista_aux[MAX] = {0};
char *tokenizar(char linea[], char *token);
int separar(char *linea, int n);

int separar(char *linea, int n)
{
  int lista[MAX] = {0};
  char *cadena;
  int contador = 0;
  cadena = strtok(linea, ",");
  while (cadena != NULL)
  {
    if (n == 3)
    {
      lista_aux[contador] = atoi(cadena);
      contador++;
      cadena = strtok(NULL, ",");
    }
    else
    {
      lista[contador] = atoi(cadena);
      contador++;
      cadena = strtok(NULL, ",");
    }
  }

  if (n == 0)
    list_insert = lista;

  if (n == 1)
    list_delete = lista;
  if (n == 2)
    list_search = lista;
  if (n == 3)
    list_update_old = lista_aux;
  if (n == 4)
    list_update_new = lista;

  return contador;
}

char *tokenizar(char linea[], char *token)
{
  strtok(linea, "\n");
  char *cadena = strtok(linea, token);
  if (cadena != NULL)
  {
    cadena = strtok(NULL, token);
    return cadena;
  }
}

void leer_archivo(char *name)
{
  FILE *archivo;
  archivo = fopen(name, "r");
  fgets(linea1, MAX, archivo);
  insert_values = tokenizar(linea1, "=");
  insert_values[strlen(insert_values) - 1] = '\0';
  printf("\nInsert values: => [%s]\n", insert_values);
  fgets(linea2, MAX, archivo);
  delete_values = tokenizar(linea2, "=");
  delete_values[strlen(delete_values) - 1] = '\0';
  printf("\nDelete values: => [%s]\n", delete_values);
  fgets(linea3, MAX, archivo);
  search_values = tokenizar(linea3, "=");
  search_values[strlen(search_values) - 1] = '\0';
  printf("\nSearch values: => [%s]\n", search_values);
  fgets(linea4, MAX, archivo);
  update_values_old = tokenizar(linea4, "=");
  update_values_old[strlen(update_values_old) - 1] = '\0';
  printf("\n   Values old: => [%s]\n", update_values_old);
  fgets(linea5, MAX, archivo);
  update_values_new = tokenizar(linea5, "=");
  update_values_new[strlen(update_values_new) - 1] = '\0';
  printf("\n   Values new: => [%s]\n\nResult:\n\n", update_values_new);
  fclose(archivo);
}

int main(int argc, char *argv[])
{
  if (argc == 2)
  {
    leer_archivo(argv[1]);

    List *list = makelist();
    int x1 = separar(insert_values, 0);

    for (int i = 0; i < x1; i++)
    {
      insert(list_insert[i], list);
    }

    int x2 = separar(delete_values, 1);
    for (int i = 0; i < x2; i++)
    {
      delete (list_delete[i], list);
    }

    int x4 = separar(search_values, 2);
    for (int i = 0; i < x4; i++)
    {
      search(list_search[i], list);
    }

    int x3 = separar(update_values_old, 3);
    separar(update_values_new, 4);

    for (int i = 0; i < x3; i++)
    {
      update(list_update_old[i], list_update_new[i], list);
    }

    display(list);
    destroy(list);
  }
  else
  {
    printf("Uso:: ./prs [ARCHIVO_TEST.TXT]\n");
  }

  return 0;
}