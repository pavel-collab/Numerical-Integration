#! /usr/bin/env bash

# в дериктории data отчищаем файлы от стары данных
for file in $(cat data_file_list.dat); do
    rm "$file"
    touch "$file"
done

# переменная, хранящая количество процессов для запуска основной программы
process_amount=2
echo "start test"

# запускаем цикл
# для количества процессов: 2, 4, 8 ...
# запускаем программу 20 раз, чтобы получить выборку данных
for file in $(cat data_file_list.dat); do
    for ((j = 0; j < 20; j+=1)) do
    mpiexec -n "$process_amount" ../src/mpi 0 10 "$file" 
    done
    process_amount=process_amount*2
done
echo "end"