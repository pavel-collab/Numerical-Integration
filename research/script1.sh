#! /usr/bin/env bash

# отчищаем файлы от старых данных
for file in $(cat data_file_list.dat)
do
rm "$file"
touch "$file"
done

# переменная, хранящая количество процессов
process_amount=2
echo "start test"

# запускаем цикл
# запускаем исполняемый файл для разного количества процессов
# в каждом эксперементе запускаем программу 20 раз, чтобы получить выборку
for file in $(cat data_file_list.dat)
do
    for ((j = 0; j < 20; j+=1))
    do
    ../build/main 0 1 "x*x" "$process_amount" "$file"
    done
    process_amount=process_amount*2
done
echo "end"