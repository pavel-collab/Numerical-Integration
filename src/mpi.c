#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <dlfcn.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "mpi.h"
//* mpic++ mpi.c -o mpi
//* mpiexec -n 2 ./mpi 
#include "../include/lib.h"

double f(double x) {return x;}

int main (int argc, char *argv[])
{
    if (argc != 3) {
        printf("Usage: %s <integration limits>", argv[0]);
        return -1;
    } 

    // integration limits
    double a = atof(argv[1]);
    double b = atof(argv[2]);
    double* buffer = (double*) malloc(2*sizeof(double));
    buffer[0] = a;
    buffer[1] = b;

    // выделяем переменную под частичную сумму и переменную под конечное значение
    double TotalSum, ProcSum = 0.0;
    // переменные под количество процессов, под текущий процесс и под количество суммируемых значений
    int ProcRank, ProcNum;
    // переменная типа MPI_Status
    MPI_Status Status;
    // инициализация
    int rc = MPI_Init (&argc, &argv);
    if (rc != MPI_SUCCESS) {
        printf ("Error starting MPI program. Terminating.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    // получаем количество процессов и номер текущего процесса
    MPI_Comm_size (MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank (MPI_COMM_WORLD, &ProcRank);

    // рассылка данных на все процессы
    MPI_Bcast (buffer, 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // вычисление частичной суммы на каждом из процессов
    double i1 = buffer[0] + ((buffer[1] - buffer[0]) / ProcNum) * ProcRank;
    double i2 = buffer[0] + ((buffer[1] - buffer[0]) / ProcNum) * (ProcRank + 1);

    // суммируем
    ProcSum = Trapez(i1, i2, 5000000, f);

    // сборка частичных сумм на процессе с рангом 0
    if (ProcRank == 0) {
        TotalSum = ProcSum;
        for (int i = 1; i < ProcNum; i++)
    	{
    	  MPI_Recv (&ProcSum, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 0,
    		    MPI_COMM_WORLD, &Status);
    	  TotalSum = TotalSum + ProcSum;
    	}
    }
    else				// все процессы отсылают свои частичные суммы
      MPI_Send (&ProcSum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);

    // вывод результата
    if (ProcRank == 0)
      printf ("\nTotal Sum = %0.2f\n", TotalSum);
    MPI_Finalize ();
    return 0;
}   
