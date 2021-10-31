## Методы численного интегрирования

### Метод правых прямоугольников

Метод численного интегрирования функции одной переменной, заключающийся в замене подынтегральной функции на многочлен нулевой степени, то есть константу, на каждом элементарном отрезке. Если рассмотреть график подынтегральной функции, то метод будет заключаться в приближённом вычислении площади под графиком суммированием площадей конечного числа прямоугольников, ширина которых будет определяться расстоянием между соответствующими соседними узлами интегрирования, а высота — значением подынтегральной функции в этих узлах.

$$ \int_a^b f(x) dx \approx f(a) (b - a) $$

### Метод левых прямоугольников

$$ \int_a^b f(x) dx \approx f(b) (b - a) $$

![Left and Right rectangular method](https://github.com/pavel-collab/Numerical-Integration/blob/main/images/LRrec.png)

### Метод прямоугольников

$$ \int_a^b f(x) dx \approx f \left(\frac{b + a}{2} \right) (b - a) $$

![Rectangular method](https://github.com/pavel-collab/Numerical-Integration/blob/main/images/rec.png)

### Метод трапеций 
Метод численного интегрирования функции одной переменной, заключающийся в замене на каждом элементарном отрезке подынтегральной функции на многочлен первой степени, то есть линейную функцию. Площадь под графиком функции аппроксимируется прямоугольными трапециями. 

$ \int_a^b f(x) dx \approx \frac{f(b) + f(a)}{2} (b - a) $

![Trapez method](https://github.com/pavel-collab/Numerical-Integration/blob/main/images/Trapes.png)

### Формула Симпсона
Формула Симпсона относится к приёмам численного интегрирования.

Суть метода заключается в приближении подынтегральной функции на отрезке $[a, b]$ интерполяционным многочленом второй степени $p_2(x)$, то есть приближение графика функции на отрезке параболой.

\[ \int_a^b f(x) dx \approx \int_a^b p_2(x) \approx \frac{b - a}{6} \left(f(a) + 4 f \left(\frac{b - a}{2} \right) + f(b) \right) \]

![](https://github.com/pavel-collab/Numerical-Integration/blob/main/images/CodeCogsEqn.gif)
![](https://github.com/pavel-collab/Numerical-Integration/blob/main/images/render.png)

<img src="https://github.com/pavel-collab/Numerical-Integration/blob/main/images/Sympson.png" alt="Sympson method" width="200"/>
<!-- ![Sympson method](https://github.com/pavel-collab/Numerical-Integration/blob/main/images/Sympson.png) -->
