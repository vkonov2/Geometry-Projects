<h1 align="left">Нахождение расстояния от точки до кривой</h1>

<h2 align="left">Постановка</h2>

Имеем кривую второго порядка в общем виде и точку на плоскости. Необходимо как можно более точно найти расстояние от точки до кривой.

<!-- <img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/f0d3c3b501848cacb86ef4eb328352338f184da4/Preliminary-Algorithms/Least-Square-Line-Fitting/images/6.jpg" alt="c" width="500" height="500"/> -->

\newpage

<h2 align="left">Алгоритм</h2>

<h3 align="left">Способ №1 - замена координат</h3>

Пусть имеем кривую второго порядка $\gamma$ в общем виде: $a x^2 + b y^2 + c xy + d x + e y + f = 0$ и точку $P_i = (x_i, y_i)$, от которой ищем расстояние до кривой $\gamma$.\\

Произведем несколько линейных замен координат, чтобы привести нашу кривую к более удобному виду.

Покажем, что подходящим поворотом осей координат можно добиться того, что $c' = 0$, где штрих означает соотвующий коэффициент уравнения кривой в новой системе координат.

Рассмотрим произвольный поворот:
$$\begin{pmatrix}
	x \\ y
\end{pmatrix} = \begin{pmatrix}
	\cos{\varphi} & -\sin{\varphi} \\
	\sin{\varphi} & \cos{\varphi}
\end{pmatrix} \begin{pmatrix}
	x' \\ y'
\end{pmatrix}$$
Подставим в уравнение:
$$\begin{gathered}
	a (\cos{\varphi} x' -\sin{\varphi}y')^2 + b (\sin{\varphi} x' + \cos{\varphi} y')^2 + c (\cos{\varphi} x' -\sin{\varphi}y') (\sin{\varphi} x' + \cos{\varphi} y') + \\
	+ d (\cos{\varphi} x' -\sin{\varphi}y') + e (\sin{\varphi} x' + \cos{\varphi} y') + f = 0
\end{gathered}$$
Коэффициент при $x'y'$ равен:
$$-2a\sin{\varphi}\cos{\varphi} + 2b\sin{\varphi}\cos{\varphi}+c(\cos^2{\varphi}-\sin^2{\varphi}) =  (b-a)\sin{2\varphi} + c \cos{2\varphi}$$
Мы хотим найти такое $\varphi$, чтобы $c'=0$, т.е.:
$$\ctg{2\varphi} = \frac{\cos{2\varphi}}{\sin{2\varphi}} = \frac{a - b}{c} \; \Rightarrow \; \varphi = \frac{1}{2}\arcctg{\frac{a - b}{c}}$$
Задача разрешима, т.к. если бы $c = 0$, то не требовалось бы никакого поворота. Т.о., в повернутой (штрихованной) системе координат кривая примет вид:
$$\begin{gathered}
	\lambda_1 x'^2 + \lambda_2 y'^2 + d' x' + e' y' + f = 0 \\
	\begin{cases}
		\lambda_1 = a \cos^2{\varphi} + b \sin^2{\varphi} + c \cos{\varphi} \sin{\varphi}\\
		\lambda_2 = a \sin^2{\varphi} + b \cos^2{\varphi} - c \cos{\varphi} \sin{\varphi}\\
		d' = d \cos{\varphi} + e \sin{\varphi} \\
		e' = -d \sin{\varphi} + e \cos{\varphi}
	\end{cases}
\end{gathered}$$

Покажем, что кривая полученного вида параллельным переносом приводится к одному из следующих видов:
1) $\lambda_1 (x'')^2 + \lambda_2 (y'')^2 + f'' = 0$, $\lambda_1, \lambda_2 \not = 0$
2) $\lambda_2 (y'')^2 + d' x'' = 0$, $\lambda_2, d' \not = 0$
3) $\lambda_2 (y'')^2 + f'' = 0$, $\lambda_2 \not = 0$

$1$: $\lambda_1, \lambda_2 \not = 0$. Тогда выделяем полные квадраты:
$$\begin{gathered}
	\lambda_1 x'^2 + \lambda_2 y'^2 + d' x' + e' y' + f = \\
	= \lambda_1 \left(x' + \frac{d'}{2\lambda_1}\right)^2 + \lambda_2 \left(y' + \frac{e'}{2\lambda_2}\right)^2 + \left(f - \frac{d'^2}{4\lambda_1} - \frac{e'^2}{4\lambda_2}\right) = \lambda_1 (x'')^2 + \lambda_2 (y'')^2 + f''
\end{gathered}$$
Имеем формулы замены координат, обратной к исходной:

$$\begin{cases}
	x'' = x' + \frac{d'}{2\lambda_1} \\
	y'' = y' + \frac{e'}{2\lambda_2}
\end{cases}$$

$2$: $\lambda_2, d' \not = 0$ (если $\lambda_2 = 0$, $\lambda_1 \not = 0$, то поменяем координаты местами). Возможны два случая:
- если $d' \not = 0$, то:
		$$\begin{gathered}
			\lambda_2 y'^2 + d'x' + e'y' + f = \\
			= \lambda_2 \left( y' + \frac{e'}{2 \lambda_2} \right)^2 + d'x' + \left( f - \frac{e'^2}{4\lambda_2} \right) = \lambda_2 (y'')^2 + d' x''
		\end{gathered}$$
		Имеем формулы замены координат, обратной к искомой: $$\displaystyle \begin{cases}
			x'' = x' + \frac{1}{d'}\left( f - \frac{e'^2}{4\lambda_2} \right) \\
			y'' = y' + \frac{e'}{2\lambda_2}
		\end{cases}$$
- если $d' = 0$, то:
		$$\begin{gathered}
			\lambda_2 y'^2 + e'y' + f = \\
			= \lambda_2 \left( y' + \frac{e'}{2 \lambda_2} \right)^2 + \left( f - \frac{e'^2}{4\lambda_2} \right) = \lambda_2 (y'')^2 + f''
		\end{gathered}$$
		Имеем формулы замены координат, обратной к искомой: $$\displaystyle \begin{cases}
			x'' = x'\\
			y'' = y' + \frac{e'}{2\lambda_2}
		\end{cases}$$

Т.к. замена координат ортогональна, то $\rho (\gamma, P_i) = \rho (\gamma'', P''_i)$. Найдем $\rho (\gamma'', P''_i)$. Составим функцию квадрата расстояния в зависимости от полученного вида кривой:
$$\begin{gathered}
	\rho^2 (\gamma_1'', P''_i) = \left( x'' - x''_i \right)^2 + \left( \pm \sqrt{\frac{1}{\lambda_2} \left( f'' - \lambda_1 (x'')^2 \right)} - y''_i \right)^2 \\
	\rho^2 (\gamma_2'', P''_i) = \left( x'' - x''_i \right)^2 + \left( \pm \sqrt{-\frac{d'}{\lambda_2} x''} - y''_i \right)^2 \\
	\rho^2 (\gamma_3'', P''_i) = \left( x'' - x''_i \right)^2 + \left( \pm \sqrt{-\frac{f''}{\lambda_2}} - y''_i \right)^2
\end{gathered}$$

Совершим параллельный перенос на вектор $(x''_i, y''_i)$, сделав эту точку новым центром координат.

Покажем изменение коэффициентов кривой при параллельном переносе на вектор $(x_i, y_i)$:
$$\begin{cases}
	x' = x - x_i \\
	y' = y - y_i
\end{cases} \; \Rightarrow \; \begin{cases}
	x = x' + x_i \\
	y = y' + y_i
\end{cases}$$

Подставим в уравнение:
$$a (x' + x_i)^2 + b (y' + y_i)^2 + c (x' + x_i) (y' + y_i) + d (x' + x_i) + e (y' + y_i) + f = 0$$

$$\begin{gathered}
	a x'^2 + b y'^2 + c x' y' + (2ax_i + cy_i + d) x' + (2by_i + cx_i + e) + \\
	+ (ax_i^2 + by_i^2 + cx_iy_i + dx_i + ey_i + f) = 0
\end{gathered}$$
Отсюда получаем уравнения на коэффициенты:
$$\begin{cases}
	a' = a \\
	b' = b \\
	c' = c \\
	d' = 2ax_i + cy_i + d \\
	e' = 2by_i + cx_i + e \\
	f' = ax_i^2 + by_i^2 + cx_iy_i + dx_i + ey_i + f
\end{cases}$$

После параллельного переноса расстояния также сохранятся, но функционал примет более удобный вид:
$$\begin{gathered}
	\rho^2 (\gamma_1'', P''_i) = \left( x'' \right)^2 + \frac{1}{\lambda_2} \left( f'' - \lambda_1 (x'')^2 \right) \\
	\rho^2 (\gamma_2'', P''_i) = \left( x'' \right)^2 -\frac{d'}{\lambda_2} x'' \\
	\rho^2 (\gamma_3'', P''_i) = \left( x'' \right)^2 -\frac{f''}{\lambda_2}
\end{gathered}$$

Найдем производную полученной функции по $x''$ и значение $x''$:
$$\begin{gathered}
	\frac{\partial}{\partial x''}\rho^2 (\gamma_1'', P''_i) = 2x'' - 2\frac{\lambda_1}{\lambda_2} x'' = 0 \; \Rightarrow \; x'' \left(1 - \frac{\lambda_1}{\lambda_2}\right) = 0\\
	\frac{\partial}{\partial x''}\rho^2 (\gamma_2'', P''_i) = 2 x'' -\frac{d'}{\lambda_2} = 0 \; \Rightarrow \; x'' = \frac{d'}{2\lambda_2}\\
	\frac{\partial}{\partial x''}\rho^2 (\gamma_3'', P''_i) = 2 x'' = 0 \; \Rightarrow \; x'' = 0
\end{gathered}$$

Тогда искомое расстояние будет равно:
$$\begin{gathered}
	\rho (\gamma_1'', P''_i) = \sqrt{ \left|\frac{f''}{\lambda_2}  \right|}, (\lambda_1 \not = \lambda_2), \; \rho (\gamma_2'', P''_i) = \sqrt{\left|-\frac{d'^2}{4\lambda_2^2}\right| }, \; \rho (\gamma_3'', P''_i) = \sqrt{ \left|-\frac{f''}{\lambda_2} \right|}
\end{gathered}$$

<h3 align="left">Способ №2 - итеративное приближение</h3>

Пусть имеем кривую второго порядка $\gamma$ в общем виде: $a x^2 + b y^2 + c xy + d x + e y + f = 0$ и точку $P_i = (x_i, y_i)$, от которой ищем расстояние до кривой $\gamma$.\\

Выполним параллельный перенос так, чтобы точки $P_i$ стала новым центром координат, то перенос на вектор $(-x_i, -y_i)$. Без ограничения общности получаем задачу поиска расстояния от точки $P'_i = (0, 0)$ до кривой $\gamma$ в общем виде: $a x^2 + b y^2 + c xy + d x + e y + f = 0$. 

Выберем сииметричный относительно нуля отрезок $[-M,M]$ и разобьем его на $N$ равных частей. Получаем равномерную сетку, узлы которой имеют вид $x^{\pm}_i = \pm i h$, где $h = \frac{2M}{N}$. Далее в алгоритме будем рассматривать узлы сетки с положительными значениями (для остальных точек алгоритм аналогичный). Будем находить ординаты точек на кривой $\gamma$, абсциссы которых являются узлы построенной сетки. Имеем квадратное уравнение по $y$:
$$\begin{gathered}
	a (i h)^2 + b y^2 + c i h y + d i h + e y + f = 0 \\
	b y^2 + (c i h + e) y + (a (i h)^2 + d i h + f) = 0
\end{gathered}$$
Дискриминант этого уравнения равен:
$$D = (c i h + e)^2 - 4b(a (i h)^2 + d i h + f)$$
\begin{itemize}
	\item[$\bullet$] 
		eсли $D < 0$, то пропускаем рассматриваемый узел сетки
	\item[$\bullet$] 
		если $D \ge 0$, то $\displaystyle y = \frac{-(cih + e) \pm \sqrt{(c i h + e)^2 - 4b(a (i h)^2 + d i h + f)}}{2b}$
\end{itemize}

Расстояния от заданных точек до искомой точки $P'_i = (0, 0)$ будет равно:
$$\rho_i = \sqrt{i^2 h^2 + \frac{\left(-(cih + e) \pm \sqrt{(c i h + e)^2 - 4b(a (i h)^2 + d i h + f)} \right)^2}{4b^2}}$$

Находим минимум среди данных расстояний $\rho_1$. Пусть без ограничения общности этот минимум достигается для некоторого узла: 
$$(ih, \frac{-(cih + e) + \sqrt{(c i h + e)^2 - 4b(a (i h)^2 + d i h + f)}}{2b})$$
Теперь рассмотрим отрезок $[ih-\frac{M}{N}; ih+\frac{M}{N}]$, проводим для него вычисления сетки с числом узлов $N$, находим минимальное среди полученных расстояний $\rho_2$. Если $\rho_2 < \rho_1$ меньше, чем наперед заданный $\varepsilon$, то алгоритм останавливается и за искомое расстояние берется $\rho_2$. Иначе итеративно будем повторять алгоритм, пока не получим указанное выше условие выхода.

Отдельной проблемой является выбор константы $M$, определяющей рассматриваемый отрезок на начальном этапе алгоритма. 

В случае замкнутых кривых (эллипса, окружности) можно найти область значений по одной из переменных через производную. Пусть без ограничения общности будем искать область значения по $x$:
$$\begin{cases}
	\frac{\partial}{\partial y}(a x^2 + b y^2 + c xy + d x + e y + f) = 0 \\
	a x^2 + b y^2 + c xy + d x + e y + f = 0
\end{cases}$$

$$\begin{cases}
	2by + cx+e = 0 \; \Rightarrow \; y = -\frac{1}{2b}(cx+e)\\
	a x^2 + b y^2 + c xy + d x + e y + f = 0
\end{cases}$$

$$\begin{gathered}
	ax^2 + b \left( -\frac{1}{2b}(cx+e) \right)^2 + c x \left( -\frac{1}{2b}(cx+e) \right) + d x + e \left( -\frac{1}{2b}(cx+e) \right) + f = 0 \\
	\left( a - \frac{c^2}{4b} \right) x^2 + \left( d-\frac{ce}{2b} \right)x + \left( f-\frac{e^2}{4b} \right) = 0 \\
	x_{1,2} = \frac{\frac{ce}{2b} - d \pm \sqrt{\left( d-\frac{ce}{2b} \right)^2 - 4\left( a - \frac{c^2}{4b} \right)\left( f-\frac{e^2}{4b} \right)}}{2a - \frac{c^2}{2b}}
\end{gathered}$$
В качестве $M$ возьмем модуль максимального из найденных $x_{1,2}$.\\

Оставшиеся случаи:
- парабола
- гипербола
- пара пересекающихся прямых
- пара параллельных прямых
- пара совпадающих прямых

Для приведенных вариантов имеет смысл применить градиентное приближение.

Заметим, что, увеличивая число узлов, мы будем повышать точность нахождения расстояния.

<h3 align="left">Способ №3 - градиентное приближение</h3>

Пусть имеем кривую второго порядка $\gamma$ в общем виде: $a x^2 + b y^2 + c xy + d x + e y + f = 0$ и точку $P_i = (x_i, y_i)$, от которой ищем расстояние до кривой $\gamma$.\\

Выполним параллельный перенос так, чтобы точки $P_i$ стала новым центром координат, то перенос на вектор $(-x_i, -y_i)$. Без ограничения общности получаем задачу поиска расстояния от точки $P'_i = (0, 0)$ до кривой $\gamma$ в общем виде: $a x^2 + b y^2 + c xy + d x + e y + f = 0$.

Уравнение вида $a x^2 + b y^2 + c xy + d x + e y + f = const$ задает семейство кривых, называемых линиями уровня функции $F(x,y) = a x^2 + b y^2 + c xy + d x + e y + f$. 

Заметим, что значение функции $F(x,y)$ в рассматриваемой точке $P_i = (0,0)$ равно $f$.

Выпишем градиент рассматриваемой функции: 
$$grad \; F = (2ax + cy + d, 2by + cx + e)$$
Известно, что вектор градиента всегда перпендикулярен линиям уровня, которые он пересекает, и он характеризует направление наибольшего возрастания расстояния от данной точки кривой.

Вычислим вектор градиента в точке $P_i$: $\displaystyle grad \; F|_{0,0} = (d, e)$. Прибавим к точке $P_i = (0,0)$ вектор $-\frac{1}{N \sqrt{d^2 + e^2}}(d,e)$, где $N$ - некоторая наперед заданная константа. 

Вычислим значение функции $F(x, y)$ в полученной точке $-\frac{1}{N \sqrt{d^2 + e^2}}(d,e)$ - пусть оно равно $f_1$. Если $|f_1| < |f|$ и они одного знака, то продолжаем итеративно описанный выше алгортим. Если на $i$-ом шаге мы обнаружим, что $f_{i+1}$ и $f_i$ разных знаков, то укорачиваем прибавляемый вектор вдвое и проводим операцию для него. Продолжаем процесс, пока длина прибавляемого вектора больше наперед заданного $\varepsilon$.

Такой итеративный переход между точками обеспечивает переход от одной линии уровня функции $F(x,y)$ до другой по направлению, характеризующемуся наикратчайшим вектором изменения расстояния для данной точки текущей линии уровня.

Сумма длин всех пройденных векторов перехода между точками будет приближенно равно реальному искомому расстоянию.\\

Заметим, что, увеличивая число $N$, мы будем повышать точность нахождения расстояния.