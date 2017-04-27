#define SIZE_ARRAY 100000 //Tamanho do array
#define N_ARRAYS 1000 // Quantidade de arrays

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* Função chamada pelo QSort
Fonte: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
 */
int cmpfunc (const void * a, const void * b){
   return ( *(int*)a - *(int*)b );
}

int main(){

  int i, j;
  clock_t start = clock();
  // Aloca as matrizes
  int (*saco_tarefas)[SIZE_ARRAY] = malloc (N_ARRAYS * sizeof *saco_tarefas);        
  //int saco_tarefas[N_ARRAYS][SIZE_ARRAY];

  // populando vetores
  for (i = 0; i < N_ARRAYS; i++){
    for(j=0; j < SIZE_ARRAY; j++){
      saco_tarefas [i][j] = 1 + ( rand() % 10000 );
    }
  }

  for (i = 0; i < N_ARRAYS; i++)
  {
  	qsort(saco_tarefas[i], SIZE_ARRAY, sizeof(int), cmpfunc);
  }

  clock_t end = clock();
  double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Execution time: [%1.2f]\n", time_spent);
}