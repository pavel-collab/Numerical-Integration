#! /usr/bin/env bash

echo "start test"

# отчищаем файл с данными
# rm data.dat

# запускаем цикл
for ((i = 2; i <= 8; i+=1))
do
/usr/bin/time -a -o time.log ../build/main 0 1 x $i
done

echo "end"