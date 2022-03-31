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

## TEST MODE
If you want switch programm to test mode, you need to uncomment #define TEST_MODE; in file long time integration. Then you have to buld the project again. After that checkout to the directory tests/ where you will find file _test.sh_.

```consol
chmod +x test.sh
./test.sh
```