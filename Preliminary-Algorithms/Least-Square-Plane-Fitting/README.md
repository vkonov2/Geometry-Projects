<h1 align="left">Построение плоскости, аппроксимирующей множество точек</h1>

<h3 align="left">Постановка задачи</h3>

Дано множество из точек в пространстве. Необходимо найти плоскость, аппроксимирущую данное множество точек с наилучшей точностью.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/33f547e8fe151cf23d36005db11a27432d161b44/Preliminary-Algorithms/Least-Square-Plane-Fitting/images/1.png" alt="c" width="500" height="500"/>

Пусть имеется набор из $n$ точек: $P = \Set{p_i = (x_i, y_i, z_i), i=1,\dots,n}$. 

Будем искать плоскость $\mathit{\pi}$ в виде уравнения: $a x + b y + c z + d = 0$. Тогда вектор нормали к плоскости будет равен $n = (a, b, c)$.

По определению расстояние от точки $M_0 = (x_0, y_0, z_0)$ до плоскости $\mathit{\pi}$ определяется по следующей формуле:

$$d (M_0, \mathit{\pi}) = \frac{| a x_0 + b y_0 + c z_0 + d |}{\sqrt{a^2 + b^2 + c^2}}$$

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/33f547e8fe151cf23d36005db11a27432d161b44/Preliminary-Algorithms/Least-Square-Plane-Fitting/images/2.jpg" alt="c" width="400" height="150"/>

Будем искать нормированное уравнение плоскости, которое получается из общего уравнения делением всех членов на $\sqrt{a^2+b^2+с,2}$: $a'x+b'y+с'+d'=0$. Тогда расстояние от точки $M_0 = (x_0,y_0, z_0)$ до плоскости равно абсолютному значению подстановки точки в уравнение плоскости и вычисляется по формуле:

$$d (M_0, \mathit{\pi}) = | a' x_0 + b' y_0 + c' z_0 + d' |$$

Тогда наша мера будет иметь вид:

$$\Lambda (\pi,P)=\sqrt{\underset{i=1}{\overset{n}{\sum}} (a'x_i+b'y_i+c'z_i+d')^2},\;(a')^2+(b')^2+(c')^2=1$$

Будем исследовать не саму меру, а подкоренное выражение, и для простоты будем рассматривать коэффициенты без индексов.

Таким образом, задача свелась к поиску минимального значения выражения $\underset{i=1}{\overset{n}{\sum}} (ax_i+by_i+cz_i+d)^2$ с ограничением $a^2+b^2+c^2=1$.

Будем искать коэффициенты методом Лагранжа. Составим функцию Лагранжа:

$$L(a, b, c, d, \lambda) = \underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)$$.

Составим систему из пяти уравнений стационарности, приравняв к нулю частные производные функции Лагранжа $L(a, b, c, d, \lambda)$ по $a, b, c, d$ и $\lambda$:

$$\begin{cases}
		\frac{\partial}{\partial a} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0 \\
		\frac{\partial}{\partial b} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0 \\
		\frac{\partial}{\partial c} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0 \\
		\frac{\partial}{\partial d} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0 \\
		\frac{\partial}{\partial \lambda} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0
	\end{cases}$$