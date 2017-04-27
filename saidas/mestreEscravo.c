#define NUM_ARRAYS 1000 // Numero de vetores 
#define SIZE_ARRAY 100000 //Numero de elementos em cada vetor
#define MASTER 0 //Salva rank do mestre
#define TAG_SEND_ARRAY 2 //Salva tag que envia array
#define TAG_RECIVE_ARRAY 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

/* Função chamada pelo QSort
Fonte: https://www.tutorialspoint.com/c_standard_library/c_function_qsort.htm
 */
int cmpfunc (const void * a, const void * b)
{
   return ( *(int*)a - *(int*)b );
}
 
main(int argc, char** argv)
{
   int my_rank;  /* Identificador do processo */
   int proc_n;   /* Número de processos */
   int source;   /* Identificador do proc.origem */
   int dest;     /* Identificador do proc. destino */
   int tag = 50; /* Tag para as mensagens */
   double t1, t2, t3, t4;

   // int sacoMaster[NUM_ARRAYS+7][SIZE_ARRAY+1]; // saco de trabalho mestre

 
   // char message[100]; /* Buffer para as mensagens */
   MPI_Status status; /* Status de retorno */
    
   MPI_Init (&argc , & argv);
    
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

   // int relProc[NUM_ARRAYS];
   // int sacoSlave[SIZE_ARRAY+1]; // array para ser ordenado escravo
   int (*sacoSlave) = malloc ((SIZE_ARRAY+1) * sizeof *sacoSlave);
   // int sacoMaster[NUM_ARRAYS][SIZE_ARRAY+1]; // saco de trabalho mestre
   int (*sacoMaster)[SIZE_ARRAY+1] = malloc ((NUM_ARRAYS) * sizeof *sacoMaster);
   //my_rank = MPI_Comm_rank();  // pega pega o numero do processo atual (rank)
   //proc_n  = MPI_Comm_size();  // pega informação do numero de processos (quantidade total)

   srand(time(NULL)); //in order to use randomic numbers
    
   if (my_rank == MASTER)
   {

      t1 = MPI_Wtime();

      // int unsorted_arrays = NUM_ARRAYS;
      /* Trabalho do mestre */
      int a, b, j, c;
      for(a=0; a<NUM_ARRAYS; a++)
      {
         for(j=0; j<SIZE_ARRAY+1; j++)
         {
            sacoMaster[a][j]= 1 + ( rand() % 100 );
         }
         sacoMaster[a][SIZE_ARRAY]=a;
      }

      // memset(&sacoMaster[NUM_ARRAYS+1][0], -1, sizeof(int)*SIZE_ARRAY*proc_n);
      // for(b = NUM_ARRAYS; b < NUM_ARRAYS + proc_n; ++b){  
      //    sacoMaster[b][SIZE_ARRAY] = -1;
      //    // printf("Saco[%d] valor [%d]\n", b, sacoMaster[b][SIZE_ARRAY]);
      // }
      // printf("VETORES CRIADOS PELO MASTER\n");


      for(b=0; b<proc_n-1; b++){
         MPI_Send(&sacoMaster[b], SIZE_ARRAY+1, MPI_INT, b+1, TAG_SEND_ARRAY, MPI_COMM_WORLD);
         //printf("VETOR[%d] ENVIADO PELO PROCESSO MASTER[%d]\n", b, my_rank);
      }

      for (b = proc_n-1; b < NUM_ARRAYS; b++) {
         MPI_Recv (sacoSlave, SIZE_ARRAY+1, MPI_INT , MPI_ANY_SOURCE, TAG_RECIVE_ARRAY, MPI_COMM_WORLD, &status);   
         //printf("VETOR[%d] recebido PROCESSO MASTER[%d] DO PROCESSO SLAVE[%d] <==========   \n", sacoSlave[SIZE_ARRAY], my_rank, status.MPI_SOURCE);      
         MPI_Send (&sacoMaster[b], SIZE_ARRAY+1, MPI_INT, status.MPI_SOURCE, TAG_SEND_ARRAY, MPI_COMM_WORLD);
         //printf("VETOR[%d] ENVIADO PELO PROCESSO MASTER[%d]\n", b, my_rank);
         // int z;
         // printf("[MESTRE %d] Vetor recebido do SLAVE: [", status.MPI_SOURCE);
         // for(z = 0; z < SIZE_ARRAY+1;++z)
         //    printf("%d ", sacoSlave[z]);
         //    //printf("P[%d]=[%d]   ", z, sacoSlave[z] );
         // printf("]\n");
         memcpy(sacoMaster[sacoSlave[SIZE_ARRAY]], sacoSlave, (SIZE_ARRAY + 1) * sizeof(int));
      }

      for(b=0; b<proc_n-1; b++)
      {

         MPI_Recv (sacoSlave, SIZE_ARRAY+1, MPI_INT , MPI_ANY_SOURCE, TAG_RECIVE_ARRAY, MPI_COMM_WORLD, &status);   
         //printf("VETOR[%d] recebido PROCESSO MASTER[%d] DO PROCESSO SLAVE[%d] <==========   \n", sacoSlave[SIZE_ARRAY], my_rank, status.MPI_SOURCE); 

         // int z;
         // printf("[MESTRE %d] Vetor recebido do SLAVE: [", status.MPI_SOURCE);
         // for(z = 0; z < SIZE_ARRAY+1;++z)
         //    printf("%d ", sacoSlave[z]);
         //    //printf("P[%d]=[%d]   ", z, sacoSlave[z] );
         // printf("]\n");
         memcpy(sacoMaster[sacoSlave[SIZE_ARRAY]], sacoSlave, (SIZE_ARRAY + 1) * sizeof(int));
      } 
      sacoSlave[SIZE_ARRAY]=-1;
      for(b = 1; b < proc_n; ++b)
      {
         MPI_Send (sacoSlave, SIZE_ARRAY+1, MPI_INT, b, TAG_SEND_ARRAY, MPI_COMM_WORLD);
      }
      t2 = MPI_Wtime();
      printf("\n********** Master [ID %d] execution time: %1.6f **********\n", my_rank, t2-t1);

      /*for(int i=1; i<proc_n; i++)
      {
         MPI_Recv (sacoMaster[i], sizeof(int), MPI_INT,i, tag, MPI_COMM_WORLD, &status);  
      }*/


      
      /*sprintf(message, "Greetings from process %d!", my_rank);
      dest = 0;
      MPI_Send (message, strlen(message)+1, MPI_CHAR,dest, tag, MPI_COMM_WORLD);*/
   }
   else
   {
      t3 = MPI_Wtime();
      while(1){
      //int (*slaveReturn) = malloc ((SIZE_ARRAY+1) * sizeof *slaveReturn);
      int slaveReturn[SIZE_ARRAY+1];
      // printf("PROCESSO ESCRAVO[%d] INICIADO\n", my_rank);
      MPI_Recv(&slaveReturn, SIZE_ARRAY+1, MPI_INT , MASTER, TAG_SEND_ARRAY, MPI_COMM_WORLD, &status);
      // printf("PROCESSO ESCRAVO[%d] RECEBEU O ARRAY\n", my_rank);
      if(slaveReturn[SIZE_ARRAY] == -1){
         break;
      }
      int z;
      //printf("[ESCRAVO %d] Vetor recebido do mestre: [", my_rank);
      // for(z = 0; z < SIZE_ARRAY+1;++z)
      //    printf("%d ", slaveReturn[z]);
      //    //printf("P[%d]=[%d]   ", z, slaveReturn[z] );
      // printf("]\n");
      
      int index = slaveReturn[SIZE_ARRAY+1];
      // if(index == -1){
         // printf("PROCESSO ESCRAVO[%d] KILLADO\n", my_rank);
         //TODO: terminar o processo escravo
      // } else {
         // slaveReturn[SIZE_ARRAY] = my_rank;

         qsort(slaveReturn, SIZE_ARRAY, sizeof(int), cmpfunc);
         MPI_Send(slaveReturn, SIZE_ARRAY+1, MPI_INT, MASTER, TAG_RECIVE_ARRAY, MPI_COMM_WORLD);
      // }


      /*for (source = 1; source < proc_n; source++)
         {
         MPI_Recv (message, 100, MPI_CHAR , source, tag, MPI_COMM_WORLD, &status);
         printf("%s\n", message);
         }*/
      //free(slaveReturn);
      }
      t4 = MPI_Wtime();

      printf("\n********** Slave [ID %d] execution time: %1.6f **********\n", my_rank, t4-t3);
   }
   // if(!my_rank){
      // int p;
      // int l;
      // printf("\n\n\n");
      // for(p = 0; p < NUM_ARRAYS; ++p){
      //    printf("VETOR[%d] - ", p);
      //    for(l = 0; l <= SIZE_ARRAY; ++l){
      //       printf("[%d] ", sacoMaster[p][l]);
      //    }
      //    printf("\n");
      // }
      MPI_Finalize();
      free(sacoMaster);
   // }
   return 0;
}
