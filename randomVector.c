#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}

int main(int argc, char *argv[] ){

   int vectorSize = atoi(argv[1]);
   int vector[vectorSize]; //vector
   int i; //count

   srand(time(NULL)); //in order to use randomic numbers

   for(i=0;i<vectorSize;i++)
   {
      vector[i]=1 + ( rand() % 10000 );                        // para colocar números aleatórios no vetor de 10 á 10
   }

   printf("\n\nVetor aleatorio\n");                       // Para escrever o vetor

   for(i=0;i<vectorSize;i++)
   {
      printf("[%d] = %d\n",i+1,vector[i]);
   }

   qsort(vector, vectorSize, sizeof(int), cmpfunc);

   printf("\n\nVetor ordenado\n");                       // Para escrever o vetor

   for(i=0;i<vectorSize;i++)
   {
      printf("[%d] = %d\n",i+1,vector[i]);
   }

   return 0;
}