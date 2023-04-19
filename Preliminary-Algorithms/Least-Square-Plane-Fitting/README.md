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

$$L(a, b, c, d, \lambda) = \underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)$$

Составим систему из пяти уравнений стационарности, приравняв к нулю частные производные функции Лагранжа $L(a, b, c, d, \lambda)$ по $a, b, c, d$ и $\lambda$:

$$\begin{cases}
		\frac{\partial}{\partial a} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0 \\
		\frac{\partial}{\partial b} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0 \\
		\frac{\partial}{\partial c} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0 \\
		\frac{\partial}{\partial d} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0 \\
		\frac{\partial}{\partial \lambda} \left(\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+cz_i+d)^2 - \lambda (a^2+b^2+c^2-1)\right) = 0
	\end{cases}, \begin{cases}
	A_{11} = \underset{i=1}{\overset{n}{\sum}}x_i^2-\frac{1}{n} \left(\underset{i=1}{\overset{n}{\sum}}x_i\right)^2 \\
	A_{12} = A_{21} = \underset{i=1}{\overset{n}{\sum}}x_i y_i - \frac{1}{n}\underset{i=1}{\overset{n}{\sum}}x_i \underset{i=1}{\overset{n}{\sum}}y_i \\
	A_{13} = A_{31} = \underset{i=1}{\overset{n}{\sum}}x_iz_i - \frac{1}{n}\underset{i=1}{\overset{n}{\sum}}x_i \underset{i=1}{\overset{n}{\sum}}z_i \\
	A_{22} = \underset{i=1}{\overset{n}{\sum}}y_i^2-\frac{1}{n} \left(\underset{i=1}{\overset{n}{\sum}}y_i\right)^2 \\
	A_{23} = A_{32} = \underset{i=1}{\overset{n}{\sum}}y_iz_i - \frac{1}{n}\underset{i=1}{\overset{n}{\sum}}y_i \underset{i=1}{\overset{n}{\sum}}z_i \\
	A_{33} = \underset{i=1}{\overset{n}{\sum}}z_i^2-\frac{1}{n} \left(\underset{i=1}{\overset{n}{\sum}}z_i\right)^2
\end{cases}$$

Решение первых трех уравнений эквивалентно решению матричного уравнения:

$$\begin{pmatrix}
	A_{11} - \lambda & A_{12} & A_{13} \\
	A_{12} & A_{22}-\lambda & A_{23} \\
	A_{13} & A_{23} & A_{33} - \lambda
\end{pmatrix}\cdot \begin{pmatrix}
	a \\ b \\ c
\end{pmatrix} = 0 \Leftrightarrow (A - \lambda E)\cdot \begin{pmatrix}
	a \\ b \\ c
\end{pmatrix} = 0,\text{ где } A = \begin{pmatrix}
	A_{11} & A_{12} & A_{13} \\
	A_{12} & A_{22} & A_{23} \\
	A_{13} & A_{23} & A_{33}
\end{pmatrix}, E = \begin{pmatrix}
	1 & 0 & 0 \\
	0 & 1 & 0 \\
	0 & 0 & 1
\end{pmatrix}$$

Тогда задача свелась к поиску собственных значений для матрицы $A$. Тем или иным способом (например, методом Якоби) находим три собственных значения: $\lambda_i, i=1,2,3$.

Получаем три системы:

$$\begin{cases}
		(A_{11} - \lambda_i ) a + A_{12} b + A_{13} c = 0 \\
		A_{12}a + (A_{22} - \lambda_i)b + A_{23}c = 0 \\
		A_{13}a + A_{23}b + (A_{33} - \lambda_i)c = 0
	\end{cases}, i = 1,2,3$$

Данная система однородная, поэтому имеет бесконечное число решений.

Находим теперь собтвенные векторы, решая данную однородную систему тем или иным способом. Получаем три вектора, каждый из которых отвечает своему собственному значению: $v_i, \; i=1,2,3$. Нормируя наши собственные векторы, мы получаем выполнения ограничения единичности (пятого уравнения системы).

Пользуясь формулой для $d$ от $a,b,c$, выведенной ранее, получаем уравнения трех плоскостей:
$$\pi_i: \; a_i x + b_i y + c_i z + d_i = 0$$

Найдем для каждой полученной плоскости значение нашего функционала:
$$L_i(a_i, b_i, c_i, d_i) = \underset{j=1}{\overset{n}{\sum}}(a_ix_j+b_iy_j+c_iz_j+d_i)^2, i=1,2,3$$

Выбираем $L = min \{L_1, L_2, L_3\}$. Без ограничения общности пусть $L_1 < L_2, \; L_1 < L_3 \Rightarrow a = a_1, b = b_1, c = c_1, d = d_1$.

Таким образом, мы однозначно нашли плоскость, которая аппроксимирует заданное множество точек в пространстве наилучшим образом.
