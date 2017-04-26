#define NUM_ARRAYS 7 // Numero de vetores 
#define SIZE_ARRAY 10 //Numero de elementos em cada vetor
#define MASTER 0 //Salva rank do mestre
#define TAG_ARRAY 2 //Salva tag que envia array

#include <stdio.h>
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
   int sacoMaster[NUM_ARRAYS][SIZE_ARRAY]; // saco de trabalho mestre
   int sacoSlave[SIZE_ARRAY]; // array para ser ordenado escravo
    
   char message[100]; /* Buffer para as mensagens */
   MPI_Status status; /* Status de retorno */
    
   MPI_Init (&argc , & argv);
    
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

   //my_rank = MPI_Comm_rank();  // pega pega o numero do processo atual (rank)
   //proc_n  = MPI_Comm_size();  // pega informação do numero de processos (quantidade total)

   srand(time(NULL)); //in order to use randomic numbers
    
   if (my_rank == MASTER)
   {

      int unsorted_arrays = NUM_ARRAYS;
      /* Trabalho do mestre */
      int a;
      int j;
      for(a=0; a<NUM_ARRAYS; a++)
      {
         for(j=0; j<SIZE_ARRAY; j++)
         {
            sacoMaster[a][j]=1 + ( rand() % 10000 );
         }
      }

      printf("VERORES CRIADOS MASTER\n");


      int b;
      for(b=1; b<proc_n; b++)
      {
         MPI_Send (&sacoMaster[b], SIZE_ARRAY, MPI_INT, b, TAG_ARRAY, MPI_COMM_WORLD);
         printf("         VETOR[%d] ENVIADO\n", b);
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
      int c;
      for(c=1; c<proc_n; c++)
      {
         MPI_Recv (&sacoSlave, SIZE_ARRAY, MPI_INT , MASTER, TAG_ARRAY, MPI_COMM_WORLD, &status);
         printf("PROCESSO ESCRAVO[%d] RECEBEU O ARRAY\n", my_rank);
      }


      /*for (source = 1; source < proc_n; source++)
         {
         MPI_Recv (message, 100, MPI_CHAR , source, tag, MPI_COMM_WORLD, &status);
         printf("%s\n", message);
         }*/
   }
   MPI_Finalize();
}
