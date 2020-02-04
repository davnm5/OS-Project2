#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "../include/list.h"
#define MAX 500
#define NTHREADS 4

typedef struct estructuraTDA
{
  int indice,indice2;
  int size,size2;
} estructura;

sem_t sem1, sem2, sem3,sem4;
char linea1[MAX], linea2[MAX], linea3[MAX], linea4[MAX], linea5[MAX];
char *insert_values, *delete_values, *search_values, *update_values_old, *update_values_new;
int list_insert[MAX], list_update[MAX],list_delete[MAX], list_search[MAX],list_update_old[MAX], list_update_new[MAX], lista_aux[MAX] = {0},lista[MAX] = {0};
char *tokenizar(char linea[], char *token);
List *list;
int array_size[5];
int separar(char *linea, int n);
void asignar(int *x,int contador,int n);
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
    asignar(list_insert,contador,0);
  if (n == 1)
    asignar(list_delete,contador,0);
  if (n == 2)
    asignar(list_search,contador,0);
  if (n == 3)
    asignar(list_update_old,contador,1);
  if (n == 4)
    asignar(list_update_new,contador,0);

  return contador;
}

void asignar(int* x,int contador,int n){
  for(int i=0;i<contador;i++){
    x[i]=lista[i];
    if(n==1){
      x[i]=lista_aux[i];
    }
    
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
       printf("Insert: %d \n", list_insert[i]);
      sem_post(&sem1);
    }
    
  }

  if (argumento->indice == 1)
  {
    
    for (int i = 0; i < argumento->size; i++)
    {
      sem_wait(&sem1);
      delete (list_delete[i], list);
      printf("Delete: %d \n", list_delete[i]);
      sem_post(&sem2);
    }
    
  }

  if (argumento->indice == 2)
  {
    
    for (int i = 0; i < argumento->size; i++)
    {
      sem_wait(&sem2);
      printf("Search: %d \n", list_search[i]);
      search(list_search[i], list);
      sem_post(&sem3);
    }
    
  }

  if (argumento->indice == 3)
  {
    
    for (int i = 0; i < argumento->size; i++)
    {
      sem_wait(&sem3);
      update(list_update_old[i],list_update_new[i],list);
      sem_post(&sem4);
    }
    
  }

  free(argumento);
  return (void *)0;
}


void crear_hilos()
{

  int status;
  

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

    if(i==3){
      array_size[i] = separar(update_values_old,i);
      separar(update_values_new,i+1);
      argumento->indice2 = i+1;
      argumento->size2 = array_size[i];
    }
    
    argumento->indice = i;
    argumento->size = array_size[i];

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
    display(list);
    destroy(list);
  }
  else
  {
    printf("Uso:: ./prs [ARCHIVO_TEST.TXT]\n");
  }

  return 0;
}