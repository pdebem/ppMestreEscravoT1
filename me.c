#include "mpi.h" 
#include <stdio.h>
#include <stdlib.h>
#include "buildVector.h"


#define TAREFAS 7 // Numero de tarefas no saco de trabalho para np = 8, processo 0 é o mestre
#define ARRAY_LENGTH 10
#define NUMBERS_OF_ARRAYS 2

int main(int argc, char **argv){

int my_rank;       // Identificador deste processo
int proc_n;        // Numero de processos disparados pelo usuário na linha de comando (np)
int message;       // Buffer para as mensagens 
int saco[TAREFAS]; // saco de trabalho
int matrix[NUMBERS_OF_ARRAYS][ARRAY_LENGTH];
MPI_Status status; // estrutura que guarda o estado de retorno          

MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o código paralelo esta abaixo

MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);  // pega pega o numero do processo atual (rank)
MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informação do numero de processos (quantidade total)

if ( my_rank == 0 ) // qual o meu papel: sou o mestre ou um dos escravos?
   {
   // papel do mestre
    int i = 0;
   for (i=0 ; i < TAREFAS; i++) // mando o trabalho para os escravos fazerem
       {
       message = saco[i];
//     MPI_Send(&message, i+1); // envio trabalho saco[i] para escravo com id = i+1;
       printf("Pid destino: %d, mensagem %d, total de processos: %d\n", i+1, saco[i+1], proc_n);
       MPI_Send(&message, ARRAY_LENGTH, MPI_INT, i+1, 1, MPI_COMM_WORLD); // envio trabalho saco[i] para escravo com id = i+1;
       } 

    // recebo o resultado
    for (i=0 ; i < TAREFAS; i++)
        {
        // recebo mensagens de qualquer emissor e com qualquer etiqueta (TAG)

        MPI_Recv(&message, ARRAY_LENGTH, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  // recebo por ordem de chegada com any_source
//      MPI_Recv(&message, 1, MPI_INT, my_rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // recebo da esquerda

        saco[status.MPI_SOURCE-1] = message;   // coloco mensagem no saco na posição do escravo emissor
        printf("Pid: %d, mensagem %d\n", i+1, saco[status.MPI_SOURCE-1]);
        }

     }              
else               
     {
     // papel do escravo

     MPI_Recv(&message, ARRAY_LENGTH, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);    // recebo do mestre
//   MPI_Recv(&message, 1, MPI_INT, my_rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // recebo da esquerda

     message = my_rank;      // icremento conteúdo da mensagem

     MPI_Send(&message, ARRAY_LENGTH, MPI_INT, 0, 1, MPI_COMM_WORLD);    // retorno resultado para o mestre
//   MPI_Send(&message, 1, MPI_INT, my_rank+1, 1, MPI_COMM_WORLD); // envio para a direita
     }

MPI_Finalize();

 }