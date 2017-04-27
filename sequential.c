#define NUM_ARRAYS 1000       /* Numero de vetores */
#define ARRAY_SIZE 100000     /* Numero de elementos em cada vetor */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* 
Função chamada pelo QSort
Fonte: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
 */
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int main()
{

  int i, j;
  clock_t begin = clock(); /* Horario de inicio */

  int (*task_bag)[ARRAY_SIZE] = malloc (NUM_ARRAYS * sizeof *task_bag); /* Alocando espaco para o vetor */

  /* Laco para popular todos as posicoes arrays com numeros aleatorios... */
  for (i = 0; i < NUM_ARRAYS; i++)
  {
    for(j=0; j < ARRAY_SIZE; j++)
    {
      task_bag [i][j] = 1 + ( rand() % 100 );
    }
  }

  /* Ordena os arays de forma sequencial */
  for (i = 0; i < NUM_ARRAYS; i++)
  {
  	qsort(task_bag[i], ARRAY_SIZE, sizeof(int), cmpfunc);
  }

  clock_t end = clock(); /* Horario de fim */
  double execution_time = (double)(end - begin) / CLOCKS_PER_SEC; /* Tempo de execucao */

  printf("********** SEQUENTIAL - Execution time: %1.2f seconds **********\n", execution_time);
}