## О программе

Основная программа получает на вход пределы интегрирования и подинтегральную функцию, после чего вычисляет численное значение интеграла в заданных пределах используя при этом несколько потоков. Потоки синхронизированы между собой с помощью мьютекса.

## Структура проекта
- include
    - lib.h -- содержит описание использующихся структур и функций
    - chrono.hpp -- содержит класс для более точного измерения времени (непосредственно в проекте не используется по причине совместимости C/C++)
- src
    - gnuplot_call.c -- программа получает на вход функцию и с помощью вызова сторонней программы gnuplot выводит на экран ее график
    - main.c -- программа получает на вход пределы интегрирования а так же подинтегральную функцию, полученная функцию численно интегрируется в несколько потоков, после этого на экран выводится результат вычислений
    - long_time_integrate.c -- программа численного интегрирования без многопоточности для сравнения прироста скорости вчислений
    - mpi.c -- программа многопоточного интегрирования с использованием технологии MPI. Получает на вход пределы интегрирования и количество потоков, на экран выводится результат вычислений интеграла от функции x*x + sqrt(abs(x)) (определена в файле mpi.c) на заданном промежутке
    - Makefile -- файл для сборки mpi.c
- tests
    - test_collection -- содержит набор файлов .dat с данными для тестирования (см TEST MODE)
    - gtest.cpp -- гугл тесты интегрирующих функций
- research
    - data -- содержит файлы с данными выборки каждого эксперемента
    - images -- содержит картинки с графиками
    - data_file_list.dat -- сожержит список файлов с данными выборок
    - pthread_test.sh -- bash-dcript для тестирования main.c
    - mpi_test.sh -- bash-dcript для тестирования mpi.c
    - PythonGraphMode.py -- модуль для построения графиков на python
    - run_graphics.py -- скрипт для построения графиков по полеченным данным

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

## RESEARCH

Для исследования времени работы программ main.c и mpi.c в проекте предусмотрено несколько дополнительных конструкций, содержащихся в дериктории research. Каждая из программ (main.c и mpi.c) имеет 2 режима работы: обычный режим, в котором рпограмма принимает на вход данные и выдает результат вычисления и решим исследования, в котором программа принимает на вход данные и записывает результат работы в специальные файлы в дериктории /research/data.

Далее речь пойдет о принципе проведения эксперементов, каждый эксперемент есть тестирование: запуск программы на определенном количестве потоков и получение времени работы программы.

### MAIN.C

В первом режиме программа принимает 3 аргумента: пределы интегрирования и подинтегральную функцию:
```
./main 0 1 "x*x"
```
в stdout выводится численное значение интеграла.

Чтобы перевести программу во второй режим перейлите в файл main.c и раскоментируйте макрос RESEARCH (1 строчка), затем пересоберите программу. В данном режиме программа принимает третьим аргументом файл, в который будет записано время работы программы. Сбор данных автоматизирован и осуществляется через скрипт pthread_test.sh в дериктории reserch.

При запуске скрипт запускает исполняемый файл на различном количестве процессов. На каждом количестве процессов программа запускается 20 раз, чтобы получить выборку времени работы. Каждая выборка записывается в соответсвующий файл reserch/data/datai.dat. Просле получения всех выборок, можно построитьт графики зависимости времени работы программы от количества процессов, спользуя скрипт run_graphics.py с аргументом pthread в дериктории reserch.
```
chmod +x pthread_test.sh
./pthread_test.sh
```
ждем, пока соберутся данные, после завершения работы скрипта:
```
chmod +x run_graphics.py
./run_graphics.py pthread
```

### MPI.C

В папке src наодится файл mpi.c, содержащий программу многопоточного интегрирования на основе тенологии MPI.

Для того, чтобы собрать файл, перейдите в папку src и выполните в консоли команду
```
make
```
Программа может работать в двух режимах: в первом режиме программа выдает в stdout результат интегрирования функции f = x*x + sqrt(abs(x)) на заданном промежутке и время работы программы. Для запуска зыполните в консоли команду
```
make test
```
при этом программа выполнит интегрирование указанной функции в пределах от 0 до 10 на 2 потоках. Для того, чтобы протестировать программу на других промежутках или на другом количестве потоков, выполните в консоли команду
```
mpiexec -n N ./mpi a b
```
где N -- количество потоков, a и b -- пределы интегрирования.

Чтобы перевести программу во второй режим перейлите в файл mpi.c и раскоментируйте макрос RESEARCH (1 строчка), затем пересоберите программу. В данном режиме программа принимает третьим аргументом файл, в который будет записано время работы программы. Сбор данных автоматизирован и осуществляется через скрипт mpi_test.sh в дериктории reserch.

При запуске скрипт запускает исполняемый файл на различном количестве процессов. На каждом количестве процессов программа запускается 20 раз, чтобы получить выборку времени работы. Каждая выборка записывается в соответсвующий файл reserch/data/datai.dat. Просле получения всех выборок, можно построитьт графики зависимости времени работы программы от количества процессов, спользуя скрипт run_graphics.py с аргументом mpi в дериктории reserch.
```
chmod +x mpi_test.sh
./mpi_test.sh
```
ждем, пока соберутся данные, после завершения работы скрипта:
```
chmod +x run_graphics.py
./run_graphics.py mpi
```