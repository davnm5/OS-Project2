#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/list.h"
#define MAX 500
#define NTHREADS 3

typedef struct estructuraTDA
{
  int indice;
  int size;
} estructura;

sem_t sem1, sem2, sem3;
char linea1[MAX], linea2[MAX], linea3[MAX], linea4[MAX], linea5[MAX];
char *insert_values, *delete_values, *search_values, *update_values_old, *update_values_new;
int list_insert[MAX], list_update[MAX],list_delete[MAX], list_search[MAX], *list_update_old, *list_update_new, lista_aux[MAX] = {0},lista[MAX] = {0};
char *tokenizar(char linea[], char *token);
List *list;
int array_size[4];
int separar(char *linea, int n);
void asignar(int *x,int contador);
int separar(char *linea, int n)
{
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
    asignar(list_insert,contador);
  if (n == 1)
    asignar(list_delete,contador);
  if (n == 2)
    asignar(list_search,contador);
  if (n == 3)
    asignar(list_update_old,contador);
  if (n == 4)
    asignar(list_update_new,contador);

  return contador;
}

void asignar(int* x,int contador){
  for(int i=0;i<contador;i++){
    x[i]=lista[i];
  }
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

void *controlador(void *arg)
{
  estructura *argumento = (estructura *)arg;
  if (argumento->indice == 0)
  {
    for (int i = 0; i < argumento->size; i++)
    {
      insert(list_insert[i], list);
       printf("insert: %d \n", list_insert[i]);
      sem_post(&sem2);
    }
    
  }

  if (argumento->indice == 1)
  {
    
    for (int i = 0; i < argumento->size; i++)
    {
      sem_wait(&sem2);
      delete (list_delete[i], list);
      printf("delete: %d \n", list_delete[i]);
      sem_post(&sem3);
    }
    
  }

  if (argumento->indice == 2)
  {
    
    for (int i = 0; i < argumento->size; i++)
    {
      sem_wait(&sem3);
      search(list_search[i], list);
      printf("search: %d \n", list_search[i]);
      sem_post(&sem3);
    }
    
  }
  free(argumento);
  return (void *)0;
}

/*
Método que se encarga de asignar a cada estructura de los hilos un inicio y fin del bloque a procesar.
*/
void crear_hilos()
{

  int status;
  
  /*array_size[3] = separar(update_values_old, 3);
  array_size[4] = separar(update_values_new, 4);*/

  pthread_t *hilos = malloc(NTHREADS * sizeof(pthread_t));
  for (int i = 0; i < NTHREADS; i++)
  {
    estructura *argumento = malloc(sizeof(estructura));
    if(i==0){
      array_size[i] = separar(insert_values, i);
    }
    if(i==1){
      array_size[i] = separar(delete_values, i);
    }
    if(i==2){
      array_size[i] = separar(search_values, i);
    }
    
    argumento->indice = i;
    argumento->size = array_size[i];
    //printf("indice: %d\n", argumento->indice);
    status = pthread_create(&hilos[i], NULL, controlador, (void *)argumento);
    if (status < 0)
    {
      fprintf(stderr, "Error al crear los hilos \n");
      exit(1);
    }
  }

  for (int j = 0; j < NTHREADS; j++)
  {
    int status = pthread_join(hilos[j], NULL); //esperando a los hilos
    if (status < 0)
    {
      fprintf(stderr, "Error al esperar por el hilo %d \n", j);
      exit(1);
    }
  }

  free(hilos); //se libera la memoria asignada a los hilos*/
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
  update_values_new[strlen(update_values_new)] = '\0';
  printf("\n   Values new: => [%s]\n\nResult:\n\n", update_values_new);
  fclose(archivo);
}

int main(int argc, char *argv[])
{
  if (argc == 2)
  {
    leer_archivo(argv[1]);

    list = makelist();
    crear_hilos();
    /*int x1 = separar(insert_values, 0);

    for (int i = 0; i < x1; i++)
    {
      insert(list_insert[i], list);
    }*/
    /*
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
*/
    display(list);
    destroy(list);
  }
  else
  {
    printf("Uso:: ./prs [ARCHIVO_TEST.TXT]\n");
  }

  return 0;
}