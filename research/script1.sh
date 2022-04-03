#! /usr/bin/env bash

for file in $(cat files.dat)
do
rm "$file"
touch "$file"
done

process_amount=2
echo "start test"
# запускаем цикл
for file in $(cat files.dat)
do
    for ((j = 0; j < 20; j+=1))
    do
    ../build/main 0 1 "x*x" "$process_amount" "$file"
    done
    process_amount+=1
done
echo "end"