## Build
Instruction how to build the project
```
mkdir build && cd build
cmake ..
make
```
After that you can run the executable file you need.  
You also can build each target separately:
```
make <target>
```
Avaliable targets:
* main
* gnuplot_call
* test
* long_time_integration

## Методы численного интегрирования

### Метод правых прямоугольников

Метод численного интегрирования функции одной переменной, заключающийся в замене подынтегральной функции на многочлен нулевой степени, то есть константу, на каждом элементарном отрезке. Если рассмотреть график подынтегральной функции, то метод будет заключаться в приближённом вычислении площади под графиком суммированием площадей конечного числа прямоугольников, ширина которых будет определяться расстоянием между соответствующими соседними узлами интегрирования, а высота — значением подынтегральной функции в этих узлах.

<p align="center">
    <img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/RightRec(eq).gif" alt="Right Rectengular method" width="200"/>
</p>

### Метод левых прямоугольников

<p align="center">
    <img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/LeftRec(eq).gif" alt="Left Rectengular method" width="200"/>
</p>

<p align="center">
    <img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/LRrec.png" alt="Right and Left Rectengular method" width="300"/>
</p>

### Метод прямоугольников

<p align="center">
    <img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/Rec(eq).gif" alt="Rectengular method" width="250"/>
</p>

<p align="center">
    <img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/rec.png" alt="Rectengular method" width="300"/>
</p>

### Метод трапеций 
Метод численного интегрирования функции одной переменной, заключающийся в замене на каждом элементарном отрезке подынтегральной функции на многочлен первой степени, то есть линейную функцию. Площадь под графиком функции аппроксимируется прямоугольными трапециями. 

<p align="center">
    <img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/Trapez(eq).gif" alt="Trapez method" width="250"/>
</p>

<p align="center">
    <img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/Trapes.png" alt="Trapez method" width="300"/>
</p>

### Формула Симпсона
Формула Симпсона относится к приёмам численного интегрирования.

Суть метода заключается в приближении подынтегральной функции на отрезке $[a, b]$ интерполяционным многочленом второй степени $p_2(x)$, то есть приближение графика функции на отрезке параболой.

<p align="center">
    <img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/Sympson(eq).gif" alt="Sympson method" width="350"/>
</p>

<p align="center">
    <img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/Sympson.png" alt="Sympson method" width="300"/>
</p>
