#! /usr/bin/env bash

for file in $(cat list.dat)
do
rm "$file"
touch "$file"
done

process_amount=2
echo "start test"
# запускаем цикл
for file in $(cat list.dat)
do
    for ((j = 0; j < 20; j+=1))
    do
    mpiexec -n "$process_amount" ../src/mpi 0 10 "$file" 
    done
    process_amount+=1
done
echo "end"