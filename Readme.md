# Стадия разработки, ветка develop

Instruction how to build the project
```consol
mkdir build && cd build
cmake ..
make
```
After that you can run the executable file you need.  
You also can build each target separately:
```consol
make <target>
```

В первом режиме программа принимает 3 аргумента: пределы интегрирования и подинтегральную функцию:
```
./main 0 1 "x*x"
```
в stdout выводится численное значение интеграла.

Чтобы перевести программу во второй режим перейлите в файл main.c и раскоментируйте макрос RESEARCH (1 строчка), затем пересоберите программу. В данном режиме программа принимает третьим аргументом файл, в который будет записано время работы программы. Сбор данных автоматизирован и осуществляется через скрипт script2.sh в дериктории reserch.

При запуске скрипт запускает исполняемый файл на различном количестве процессов. На каждом количестве процессов программа запускается 20 раз, чтобы получить выборку времени работы. Каждая выборка записывается в соответсвующий файл reserch/data/datai.dat. Просле получения всех выборок, можно построитьт графики зависимости времени работы программы от количества процессов, спользуя скрипт script1.py в дериктории reserch.
```
chmod +x script1.sh
./script1.sh
```
ждем, пока соберутся данные, после завершения работы скрипта:
```
chmod +x script.py
./script.py
```

## О программе

Основная программа получает на вход пределы интегрирования и подинтегральную функцию, после чего вычисляет численное значение интеграла в заданных пределах используя при этом несколько потоков. Потоки синхронизированы между собой с помощью мьютекса.

## Структура проекта
- include
    - lib.h -- содержит описание использующихся структур и функций
- src
    - gnuplot_call.c -- программа получает на вход функцию и с помощью вызова сторонней программы gnuplot выводит на экран ее график
    - main.c -- программа получает на вход пределы интегрирования а так же подинтегральную функцию, полученная функцию численно интегрируется в несколько потоков, после этого на экран выводится результат вычислений
    - long_time_integrate.c -- программа численного интегрирования без многопоточности для сравнения прироста скорости вчислений
- tests
    - test_collection -- содержит набор файлов .dat с данными для тестирования (см TEST MODE)
    - gtest.cpp -- гугл тесты интегрирующих функций

## Build
Instruction how to build the project
```consol
mkdir build && cd build
cmake ..
make
```
After that you can run the executable file you need.  
You also can build each target separately:
```consol
make <target>
```
Avaliable targets:
* main
* gnuplot_call
* runUnitTests
* long_time_integration

Формат запуска:
```
./main 0 1 "sin(x)"
./gnuplot_call "sin(x)"
./runUnitTests
```

## TEST MODE
If you want switch programm to test mode, you need to uncomment #define TEST_MODE; in file long time integration. Then you have to buld the project again. After that checkout to the directory tests/ where you will find file _test.sh_.

```consol
chmod +x test.sh
./test.sh
```