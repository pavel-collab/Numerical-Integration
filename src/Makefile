mpi: mpi.c
	mpic++ mpi.c -o mpi

test:
	mpiexec -n 2 ./mpi 0 10

clean:
	rm mpi