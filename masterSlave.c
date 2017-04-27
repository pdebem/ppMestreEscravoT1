#define NUM_ARRAYS 1000       /* Numero de vetores */
#define ARRAY_SIZE 100000     /* Numero de elementos em cada vetor */
#define MASTER 0              /* Rank do mestre */
#define SEND_ARRAY 2          /* Tag que representa envio do mestre para os escravos */
#define RECEIVE_ARRAY 2       /* Tag que representa envio dos escravos para o mestre */

#include <stdio.h>     
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mpi.h"

/* 
Função auxiliar chamada pelo QSort
Fonte: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
 */
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
 
int main(int argc, char** argv)
{
   int process_id, num_processes;    /* Identificador do processo / Numero de processos */
   double t1, t2, t3, t4;            /* Marcadores de tempo */

   MPI_Status status; /* Status de retorno */
    
   MPI_Init (&argc , & argv);
    
   MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
   MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

   int returns[num_processes];

   int (*current_array) = malloc ((ARRAY_SIZE+1) * sizeof *current_array);          /* Alocando espaco para o vetor */
   int (*master_bag)[ARRAY_SIZE+1] = malloc ((NUM_ARRAYS) * sizeof *master_bag);    /* Alocando espaco para o saco do mestre */

   srand(time(NULL)); // Para gerar numeros aleatorios
    
   if (process_id == MASTER) 
   {
      /* Trabalho do mestre */

      t1 = MPI_Wtime();
      int a, b, j, c;

      /* Laco para popular todos as posicoes arrays com numeros aleatorios... */
      for(a=0; a<NUM_ARRAYS; a++)
      {
         for(j=0; j<ARRAY_SIZE+1; j++)
         {
            master_bag[a][j]= 1 + ( rand() % 100 );
         }
         master_bag[a][ARRAY_SIZE]=a; /* exceto na ultima, na qual se guarda o ID do array */
      }

      /* Laco inicial que envia um array para cada um dos escravos */         
      for(b=0; b<num_processes-1; b++)
      {
         MPI_Send(&master_bag[b], ARRAY_SIZE+1, MPI_INT, b+1, SEND_ARRAY, MPI_COMM_WORLD);
      }

      /* Recebe array ordenado e envia outro para o escravo ordenar */
      for (b = num_processes-1; b < NUM_ARRAYS; b++) 
      {
         MPI_Recv (current_array, ARRAY_SIZE+1, MPI_INT , MPI_ANY_SOURCE, RECEIVE_ARRAY, MPI_COMM_WORLD, &status);
         ++returns[status.MPI_SOURCE];
         MPI_Send (&master_bag[b], ARRAY_SIZE+1, MPI_INT, status.MPI_SOURCE, SEND_ARRAY, MPI_COMM_WORLD);
         memcpy(master_bag[current_array[ARRAY_SIZE]], current_array, (ARRAY_SIZE + 1) * sizeof(int));
      }

      /* Recebe o ultimo array ordenado de cada escravo */
      for(b=0; b<num_processes-1; b++)
      {
         MPI_Recv (current_array, ARRAY_SIZE+1, MPI_INT , MPI_ANY_SOURCE, RECEIVE_ARRAY, MPI_COMM_WORLD, &status);   
         ++returns[status.MPI_SOURCE];
         memcpy(master_bag[current_array[ARRAY_SIZE]], current_array, (ARRAY_SIZE + 1) * sizeof(int));
      } 

      current_array[ARRAY_SIZE]=-1; /* Altera valor para encerrar execucao */

      /* Envia mensagem de "fim" para os escravos */
      for(b = 1; b < num_processes; ++b)
      {
         MPI_Send (current_array, ARRAY_SIZE+1, MPI_INT, b, SEND_ARRAY, MPI_COMM_WORLD);
      }

      t2 = MPI_Wtime();
      printf("********** PARALLEL - Master [ID %d] execution time: %1.6f seconds **********\n", process_id, t2-t1);         
   }
   else
   {
      t3 = MPI_Wtime();

      /* Executa enquanto houver arrays para ordenar */
      while(1)
      {
         int slave_buffer[ARRAY_SIZE+1];
         MPI_Recv(&slave_buffer, ARRAY_SIZE+1, MPI_INT , MASTER, SEND_ARRAY, MPI_COMM_WORLD, &status); /* Recebe array para ser ordenado */

         /* Quando nao ha mais arrays para ordenar, para */
         if(slave_buffer[ARRAY_SIZE] == -1)
         {
            break;
         }

         int z;
         int index = slave_buffer[ARRAY_SIZE+1];

         qsort(slave_buffer, ARRAY_SIZE, sizeof(int), cmpfunc);                                 /* Ordena array */
         MPI_Send(slave_buffer, ARRAY_SIZE+1, MPI_INT, MASTER, RECEIVE_ARRAY, MPI_COMM_WORLD);  /* Manda array ordenado de volta para o mestre */
         
      }

      t4 = MPI_Wtime();
      printf("********** PARALLEL - Slave [ID %d] execution time: %1.6f seconds **********\n", process_id, t4-t3);
   }

   if(!process_id){
      for(i = 1; i < num_processes; ++i){
         printf("Processo[%d] realizou[%d] ordenamentos!\n",i, returns[i]);
      }
   }
   MPI_Finalize();
   free(master_bag);
   return 0;
}
