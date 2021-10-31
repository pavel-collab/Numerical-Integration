## Методы численного интегрирования

### Метод правых прямоугольников

Метод численного интегрирования функции одной переменной, заключающийся в замене подынтегральной функции на многочлен нулевой степени, то есть константу, на каждом элементарном отрезке. Если рассмотреть график подынтегральной функции, то метод будет заключаться в приближённом вычислении площади под графиком суммированием площадей конечного числа прямоугольников, ширина которых будет определяться расстоянием между соответствующими соседними узлами интегрирования, а высота — значением подынтегральной функции в этих узлах.

$$ \int_a^b f(x) dx \approx f(a) (b - a) $$

### Метод левых прямоугольников

$$ \int_a^b f(x) dx \approx f(b) (b - a) $$

### Метод прямоугольников

$$ \int_a^b f(x) dx \approx f \left(\frac{b + a}{2} \right) (b - a) $$

### Метод трапеций 
Метод численного интегрирования функции одной переменной, заключающийся в замене на каждом элементарном отрезке подынтегральной функции на многочлен первой степени, то есть линейную функцию. Площадь под графиком функции аппроксимируется прямоугольными трапециями. 

$$ \int_a^b f(x) dx \approx \frac{f(b) + f(a)}{2} (b - a) $$

### Формула Симпсона
Формула Симпсона относится к приёмам численного интегрирования.

Суть метода заключается в приближении подынтегральной функции на отрезке $[a, b]$ интерполяционным многочленом второй степени $p_2(x)$, то есть приближение графика функции на отрезке параболой.

$$ \int_a^b f(x) dx \approx \int_a^b p_2(x) \approx \frac{b - a}{6} \left(f(a) + 4 f \left(\frac{b - a}{2} \right) + f(b) \right) $$
