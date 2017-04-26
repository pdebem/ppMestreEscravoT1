#define NUM_ARRAYS 7 // Numero de vetores 
#define SIZE_ARRAY 10 //Numero de elementos em cada vetor
#define MASTER 0 //Salva rank do mestre
#define TAG_SEND_ARRAY 2 //Salva tag que envia array
#define TAG_RECIVE_ARRAY 3
#include <stdio.h>
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

   // int sacoMaster[NUM_ARRAYS+7][SIZE_ARRAY+2]; // saco de trabalho mestre
   int sacoSlave[SIZE_ARRAY+2]; // array para ser ordenado escravo
    
   // char message[100]; /* Buffer para as mensagens */
   MPI_Status status; /* Status de retorno */
    
   MPI_Init (&argc , & argv);
    
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

   int relProc[NUM_ARRAYS];
   int sacoMaster[NUM_ARRAYS+proc_n][SIZE_ARRAY+2]; // saco de trabalho mestre
   //my_rank = MPI_Comm_rank();  // pega pega o numero do processo atual (rank)
   //proc_n  = MPI_Comm_size();  // pega informação do numero de processos (quantidade total)

   srand(time(NULL)); //in order to use randomic numbers
    
   if (my_rank == MASTER)
   {

      // int unsorted_arrays = NUM_ARRAYS;
      /* Trabalho do mestre */
      int a;
      int b;
      int j;
      int c;
      for(a=0; a<NUM_ARRAYS; a++)
      {
         for(j=0; j<SIZE_ARRAY+2; j++)
         {
            sacoMaster[a][j]=1 + ( rand() % 10000 );
         }
         sacoMaster[a][j]=a;
      }

      // memset(&sacoMaster[NUM_ARRAYS+1][0], -1, sizeof(int)*SIZE_ARRAY*proc_n);
      for(b = NUM_ARRAYS+1; b < NUM_ARRAYS + proc_n; ++b){  
         sacoMaster[b][SIZE_ARRAY+2] = -1;
         printf("Saco[%d] valor [%d]\n", b, sacoMaster[b][SIZE_ARRAY+2]);
      }
      printf("VERORES CRIADOS MASTER\n");


      for(b=0; b<proc_n; b++)
      {
         MPI_Send (&sacoMaster[b], SIZE_ARRAY+2, MPI_INT, b, TAG_SEND_ARRAY, MPI_COMM_WORLD);
         printf("VETOR[%d] ENVIADO PROCESSO MASTER[%d]\n", b, my_rank);
      }
      for(b = proc_n; b<NUM_ARRAYS; b++)
      {
         MPI_Recv (sacoSlave, SIZE_ARRAY+2, MPI_INT , MPI_ANY_SOURCE, TAG_RECIVE_ARRAY, MPI_COMM_WORLD, &status);   
         printf("VETOR[%d] recebido PROCESSO MASTER[%d] DO PROCESSO SLAVE[%d]\n", sacoSlave[SIZE_ARRAY+1], my_rank, sacoSlave[SIZE_ARRAY+2]);      
         MPI_Send (sacoMaster[b], SIZE_ARRAY+2, MPI_INT, sacoSlave[SIZE_ARRAY+2], TAG_SEND_ARRAY, MPI_COMM_WORLD);
         printf("VETOR[%d] ENVIADO PROCESSO MASTER[%d]\n", b, my_rank);
         
      }

      for(b=0; b<proc_n; b++)
      {
         MPI_Recv (sacoSlave, SIZE_ARRAY+2, MPI_INT , MPI_ANY_SOURCE, TAG_SEND_ARRAY, MPI_COMM_WORLD, &status);   
         printf("VETOR[%d] recebido PROCESSO MASTER[%d] DO PROCESSO SLAVE[%d]\n", sacoSlave[SIZE_ARRAY+1], my_rank, sacoSlave[SIZE_ARRAY+2]); 
      } 

      for(b = 1; b <= proc_n; ++b)
      {
         MPI_Send (&sacoMaster[b+NUM_ARRAYS], SIZE_ARRAY+2, MPI_INT, b, TAG_SEND_ARRAY, MPI_COMM_WORLD);
         printf("KILL VETOR[%d] ENVIADO PROCESSO MASTER[%d]\n", b, my_rank);         
      }


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
      printf("PROCESSO ESCRAVO[%d] INICIADO\n", my_rank);

      MPI_Recv (sacoSlave, SIZE_ARRAY+2, MPI_INT , MASTER, TAG_SEND_ARRAY, MPI_COMM_WORLD, &status);
      printf("PROCESSO ESCRAVO[%d] RECEBEU O ARRAY\n", my_rank);
      if(sacoSlave[SIZE_ARRAY+1]==-1){
         printf("PROCESSO ESCRAVO[%d] KILLADO\n", my_rank);
         //TODO: terminar o processo escravo
      } else {
         sacoSlave[SIZE_ARRAY+2] = my_rank;
         //qsort(sacoSlave, SIZE_ARRAY, sizeof(int), cmpfunc);
         MPI_Send (&sacoSlave, SIZE_ARRAY+2, MPI_INT, 1, TAG_SEND_ARRAY, MPI_COMM_WORLD);
      }


      /*for (source = 1; source < proc_n; source++)
         {
         MPI_Recv (message, 100, MPI_CHAR , source, tag, MPI_COMM_WORLD, &status);
         printf("%s\n", message);
         }*/
   }
   MPI_Finalize();
}
