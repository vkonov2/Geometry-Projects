<h1 align="left">Построение эллипса, аппроксимирующего множество точек</h1>

<h3 align="left">Постановка задачи</h3>

Дано множество из точек в плоскости. Необходимо построить эллипс, максимально близкий к заданному множеству. В качестве основной меры близости выберем величину, равную корню из суммы квадратов расстояний от точек множества до прямой. Соответственно, необходимо подобрать такие коэффициенты уравнения кривой второго порядка, чтобы это кривая являлась эллипсом и для данного набора точек мера на ней была минимальна.

<!-- <img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/f0d3c3b501848cacb86ef4eb328352338f184da4/Preliminary-Algorithms/Least-Square-Line-Fitting/images/6.jpg" alt="c" width="500" height="500"/> -->

<h2 align="left">Основные сведения</h2>

Эллипс $C$ может быть представлен в координатах $x$ и $y$ в следующем виде:
$$C(x,y) = \boldsymbol{a} x^2 + 2\boldsymbol{b} xy + \boldsymbol{c}y^2 + 2 \boldsymbol{d} x + 2 \boldsymbol{e} y + \boldsymbol{f} = 0$$
Здесь $a$, $b$ и $c$ не могут быть равны нулю одновременно. Также для эллипса необходимо ограничение $\boldsymbol{b}^2-\boldsymbol{a}\boldsymbol{c} < 0$.

Целью является определить коэффициенты $\boldsymbol{a}, \boldsymbol{b}, \boldsymbol{c}, \boldsymbol{d}, \boldsymbol{e}$ и $\boldsymbol{f}$ так, чтобы полученный эллипс наилучшим образом описывал исходные данные в определении критерия качетсва.\\

Чтобы отбросить тривиальное решение $\boldsymbol{a} = \boldsymbol{b} = \boldsymbol{c} = \boldsymbol{d} = \boldsymbol{e} = \boldsymbol{f} = 0$, обычно используют одно из трех следующих ограничений:
- ограничение $\boldsymbol{a}+\boldsymbol{c}=1$
- ограничение $\boldsymbol{a}^2 + \boldsymbol{b}^2 + \boldsymbol{c}^2 + \boldsymbol{d}^2 + \boldsymbol{e}^2 + \boldsymbol{f}^2 = 1$
- ограничение $f = 1$

Также имеем полезную форму записи уравнения эллипса в параметрическом виде:

$$\begin{cases}
	x(t) = x_c + a \cos{t} \cos{\alpha} - b \sin{t} \sin{\alpha} \\
	y(t) = y_c + a \cos{t} \sin{\alpha} + b \sin{t} \cos{\alpha}
\end{cases}$$

где параметр $t \in [0, 2\pi]$, $(x_c, y_c)$ - центр эллипса, $a$ и $b$ - большая и малая полуоси эллипса, $\alpha$ - угол между осью $OX$ и полуосью $a$.

Более того, эллипс может быть представлен без поворота и с центром в начале координат. Соответствующую систему координат будем обозначать $XY$, чтобы отличить ее от основной системы координат $xy$. Переход от системы $xy$ к системе $XY$ осуществляется с помощью матрицы поворота:

$$\boldsymbol{R} = \begin{pmatrix}
	\cos{\alpha} & \sin{\alpha} \\
	-\sin{\alpha} & \cos{\alpha}
\end{pmatrix}$$

Пусть $\boldsymbol{x} = (x, y)^T$ - точка в системе $xy$, $\boldsymbol{X} = (X,Y)^T$ - точка в системе $XY$. Тогда точка $\boldsymbol{X}$ будет выражаться через $\boldsymbol{x}$ и обратно следующим образом:

$$\boldsymbol{X} = \boldsymbol{R} (\boldsymbol{x} - \boldsymbol{x}_c), \boldsymbol{x} = \boldsymbol{R}^{-1} \boldsymbol{X} + \boldsymbol{x}_c$$

<h2 align="left">LSA-алгоритм</h2>

Выберем нормировку коэффициентов в виде $a+c = 1$, тогда уравнение эллипса будет иметь вид:
$$C(x,y) = \boldsymbol{a}(x^2 - y^2) + 2 \boldsymbol{b}x y + 2 \boldsymbol{d} x + 2 \boldsymbol{e} y + y^2 + \boldsymbol{f}$$
В векторной форме имеем:

$$\begin{gathered}
	C(x,y) = \phi \boldsymbol{p} - z \\
	\phi = (x^2, y^2, 2xy, 2x, 2y, 1),  \boldsymbol{p} = (\boldsymbol{a}, \boldsymbol{b}, \boldsymbol{d}, \boldsymbol{e}, \boldsymbol{f})^T,  z = -y^2 
\end{gathered}$$

Имея $n$ точек $\{(x_1, y_1), (x_2, y_2), \dots, (x_n, y_n)\}$, получаем систему уравнений:

$$\begin{cases}
	C_1 (x_1, y_1) = \phi_1 \boldsymbol{p} - z_1 \\
	C_2 (x_2, y_2) = \phi_2 \boldsymbol{p} - z_2 \\
	\vdots \dots \dots \vdots \\
	C_n (x_n, y_n) = \phi_n \boldsymbol{p} - z_n
\end{cases}$$

В векторной форме имеем:

$$\begin{gathered}
	\boldsymbol{C} = \boldsymbol{\Phi} \boldsymbol{p} - \boldsymbol{y} \\
	\text{где } \boldsymbol{C} = \left( C_1(x_1, y_1), C_2(x_2, y_2), \dots, C_n(x_n, y_n) \right)^T, \\
	\boldsymbol{\Phi} = (\phi_1^T, \phi_2^T, \dots, \phi_n^T)^T, \boldsymbol{y} = (z_1, z_2, \dots, z_n)^T
\end{gathered}$$

Необходимо найти $\boldsymbol{p}$, который оптимально удовлетворяет всем точкам согласно методу наименьших квадратов. Так точки являются неточными измерениями, то не существует такого $\boldsymbol{p}$, который зануляет $\boldsymbol{C}$, т.е. всегда будет оставаться вектор ошибок $\boldsymbol{\tilde{e}} = (\tilde{e}_1, \tilde{e}_2, \dots, \tilde{e}_n)^T$. Таким образом, реальная задача состоит в минимизации вектора ошибок $\boldsymbol{\tilde{e}}$ с помощью правильного выбора $\boldsymbol{p}$. Для этого линейная регрессия находит вектор $\boldsymbol{p}$, минимизируя функцию полезности:

$$\boldsymbol{G}(\boldsymbol{p}) = \boldsymbol{C}^T \boldsymbol{C} = \boldsymbol{\tilde{e}}^T \boldsymbol{\tilde{e}} = (\boldsymbol{\Phi} \boldsymbol{p} - \boldsymbol{y})^T(\boldsymbol{\Phi} \boldsymbol{p} - \boldsymbol{y})$$

Приравнивая производную функцию полезности к нулю, получаем:

$$\begin{gathered}
	\frac{\partial}{\partial \boldsymbol{p}} \boldsymbol{G}(\boldsymbol{p}) = 0 \Rightarrow \frac{\partial}{\partial \boldsymbol{p}} (\boldsymbol{\Phi} \boldsymbol{p} - \boldsymbol{y})^T(\boldsymbol{\Phi} \boldsymbol{p} - \boldsymbol{y}) = 0 \Rightarrow \\
	\Rightarrow 2 \boldsymbol{\Phi}^T (\boldsymbol{\Phi} \boldsymbol{p} - \boldsymbol{y}) = 0 \Rightarrow \boldsymbol{p} = (\boldsymbol{\Phi}^T \boldsymbol{\Phi})^{-1} \boldsymbol{\Phi}^T \boldsymbol{y}
\end{gathered}$$

Таким образом, получаем $\boldsymbol{p}$ - решение, доставляющее минимальную $L_2$-ошибку по отношению к алгебраической дистанции.

Точки лежат на исходном эллипсе:

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSA/EllipseQuarterLSA.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSA/EllipseHalfLSA.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSA/EllipseWholeLSA.png" alt="c" width="300" height="300"/>

Точки разбросаны равномерно:

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSA/EllipseQuarterUniformLSA.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSA/EllipseHalfUniformLSA.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSA/EllipseWholeUniformLSA.png" alt="c" width="300" height="300"/>

Точки разбросаны нормально:

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSA/EllipseQuarterNormLSA.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSA/EllipseHalfNormLSA.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSA/EllipseWholeNormLSA.png" alt="c" width="300" height="300"/>

<h2 align="left">LSO-алгоритм</h2>

<h3>Идея</h3>

В LSA-алгоритме минимизировалось алгебраическое расстояние $C(x, y)$. Это привело к компактному и простому в вычислении решению $\boldsymbol{p}$. Однако решение методом наименьших квадратов, основанное на алгебраическом расстоянии, не совсем верно. Ошибка, допущенная в этом методе, называется high/low curvative bias. Говоря простыми словами, смещение возникает из-за заданной формы эллипса. Таким образом, измерение вдоль большой полуоси $a$ имеет большое значение для $C(x,y)$, тогда как измерение вдоль малой полуоси $b$ имеет меньшее значение. Однако оба измерения могут быть одинаково близки к эллипсу, хотя их расстояние $C(x,y)$ различно. Чтобы избежать этого смещения, необходимо использовать ортогональное расстояние между измерением $(x_i, y_i)$ и эллипсом.

Ортогональное расстояние $d$ — это кратчайшее соединение между точкой $(x_i, y_i)$ и эллипсом. Вектор параметров $\boldsymbol{p}$ можно получить путем минимизации суммы квадратов расстояний:

$$\boldsymbol{G}(\boldsymbol{p}) = \underset{i=1}{\overset{n}{\sum}}d_i^2$$

Для двумерного случая нахождение расстояния d является задачей решения полинома 4-й степени и может быть решено аналитически. Однако аналитическое решение неустойчиво при измерениях $|x_i| \approx 0$ или $|y_i| \approx 0$, поэтому будем использовать решение задачи нахождения расстояния, использующее алгоритм Гаусса-Ньютона. Как только ортогональное расстояние $d_i$ для каждой точки $i$ найдено, функция полезности $\boldsymbol{G}$ должна быть минимизирована, чтобы найти вектор параметров $\boldsymbol{p}$ с наименьшей ошибкой. Для этой цели подходят итерационные алгоритмы (Гаусс-Ньютон, градиентный спуск и другие). Мы будем использовать подход Гаусса-Ньютона.

<h3>Нахождение ортогональной точки на эллипсе</h3>

Чтобы получить ортогональное расстояние $d$, вычисляется соответствующая ортогональная точка $(x_{\perp}, y_{\perp})$ на эллипсе. Расчет упрощается, если это делать в системе координат $XY$, т.е. в системе координат, в которой эллипс не повернут и симметричен относительно каждой оси координат. Эллиптическое уравнение для этого случая имеет вид:

$$\frac{X^2}{a^2} + \frac{Y^2}{b^2} = 1$$

Иначе это уравнение можно записать в следующем виде:

$$f_1(X,Y) = \frac{1}{2}(a^2Y^2 +b^2X^2 −a^2b^2)=0$$

Для условия, что касательная ортогональной точки $(X_{\perp}, Y_{\perp})$ ортогональна измерению $(X_i, Y_i)$:

$$\frac{dy}{dx} \cdot \frac{Y_i - Y_{\perp}}{X_i - X_{\perp}} = \frac{-b^2 X_{\perp}}{a^2 Y_{\perp}} \cdot \frac{Y_i - Y_{\perp}}{X_i - X_{\perp}} = -1$$

Отсюда получаем второе уравнение:

$$f_2(X,Y) = b^2X(Y_i −Y)−a^2Y(X_i −X)=0$$

Имеем систему уравнений, которую можно решить в векторной форме:

$$\boldsymbol{f}(X,Y) = \begin{pmatrix}
	f_1(X,Y) \\ f_2(X,Y)
\end{pmatrix}$$

Это возможно аналитически, но, как было упомянуто, такое решение численно неустойчиво. Поэтому в дальнейшем к системе уравнений будет применяться метод Гаусса-Ньютона. Для этого вычисляется матрица Якоби:

$$\boldsymbol{Q} = \begin{pmatrix}
	\frac{\partial f_1}{\partial x} & \frac{\partial f_1}{\partial y} \\
	\frac{\partial f_2}{\partial x} & \frac{\partial f_2}{\partial y} 
\end{pmatrix} = \begin{pmatrix}
		b^2 x & a^2 y \\
		(a^2 - b^2)y + b^2 y_i & (a^2 - b^2) x - a^2 x_i
\end{pmatrix}$$

Начальное приближение $\boldsymbol{X}_0$ предлагается вычислять через усреднение некоторых точек $\boldsymbol{X}_{k_1}$ и $\boldsymbol{X}_{k_2}$:

$$\begin{gathered}
	\boldsymbol{X}_0 = \frac{1}{2} (\boldsymbol{X}_{k_1} + \boldsymbol{X}_{k_2}) \\
	\boldsymbol{X}_{k_1} = \frac{ab}{\sqrt{b^2 X_i^2 + a^2 Y_i^2}} \begin{pmatrix}
		X_i \\ Y_i
	\end{pmatrix}, \boldsymbol{X}_{k_2} = \begin{cases}
		\begin{pmatrix}
			X_i \\ sgn(Y_i) \frac{b}{a} \sqrt{a^2 - X_i^2}
		\end{pmatrix}, \text{если } |X_i| < a \\
		\begin{pmatrix}
			sgn(X_i)a \\ 0
		\end{pmatrix}, \text{если } |X_i| \ge a
	\end{cases}
\end{gathered}$$

Ортогональная точка $\boldsymbol{X}' = (X_{\perp}, Y_{\perp})^T$ вычисляется итеративно:

$$\boldsymbol{X}'_{k+1} = \boldsymbol{X}'_k - \boldsymbol{Q}_k^{-1} \boldsymbol{f}(X_k)$$

Наконец, путем обратного преобразования в систему координат $xy$ можно определить искомый вектор ортогонального расстояния:

$$\boldsymbol{d}_i = \boldsymbol{x}_i - \boldsymbol{x}'_i$$

<h3>Нахождение эллипса по ортогональным расстояниям</h3>

Имея ортогональную точку $\boldsymbol{X}'$ теперь можно выполнить алгоритм нахождения эллипса также с помощью алгоритма Гаусса-Ньютона. Для этого определяется вектор $\boldsymbol{q} = (x_c, y_c, a, b, \alpha)^T$, содержащий искомые параметры. Для каждого измерения $\boldsymbol{x}_i$ вычисляется матрица Якоби:

$$\boldsymbol{J}_{\boldsymbol{x} = \boldsymbol{x}'_i} = \boldsymbol{R}^{-1} \boldsymbol{Q}^{-1} \boldsymbol{B}$$

Матрица $B$ вычисляется следующим образом:

$$\begin{gathered}
	\boldsymbol{B} = \boldsymbol{B}_1 \boldsymbol{B}_2 \boldsymbol{B}_3 \boldsymbol{B}_4 \\
	\begin{cases}
		\boldsymbol{B}_1 = \begin{pmatrix}
			b^2 x \cos{\alpha} - a^2 y \sin{\alpha} \\
			b^2 (y_i-y) \cos{\alpha} - a^2 (x_i-x) \sin{\alpha}
		\end{pmatrix}\\
		\boldsymbol{B}_2 = \begin{pmatrix}
			b^2 x \sin{\alpha} + a^2 y \cos{\alpha} \\
			b^2 (y_i-y) \sin{\alpha} - a^2 (x_i-x) \cos{\alpha}
		\end{pmatrix} \\
		\boldsymbol{B}_3 = \begin{pmatrix}
			a (b^2 - y^2) \\ 2ay(x_i - x)
		\end{pmatrix}\\
		\boldsymbol{B}_4 = \begin{pmatrix}
			b(a^2-x^2) \\ -2bx(y_i-y)
		\end{pmatrix}
	\end{cases}
\end{gathered}$$

Затем итеративно определяется искомый вектор параметров $\boldsymbol{q}$ с шагом $\lambda$:

$$\boldsymbol{q}_{k+1} = \boldsymbol{q}_k + \lambda \Delta \boldsymbol{q}$$

Получаем систему:

$$\begin{pmatrix}
	\boldsymbol{J}_{\boldsymbol{x} = \boldsymbol{x}'_1}\\
	\boldsymbol{J}_{\boldsymbol{x} = \boldsymbol{x}'_2} \\
	\vdots \\
	\boldsymbol{J}_{\boldsymbol{x} = \boldsymbol{x}'_n}
\end{pmatrix} \Delta \boldsymbol{q} = \begin{pmatrix}
	\boldsymbol{d}_1 \\
	\boldsymbol{d}_2 \\
	\vdots \\
	\boldsymbol{d}_n
\end{pmatrix}$$

Точки лежат на исходном эллипсе:

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSO/EllipseQuarterLSO.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSO/EllipseHalfLSO.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSO/EllipseWholeLSO.png" alt="c" width="300" height="300"/>

Точки разбросаны равномерно:

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSO/EllipseQuarterUniformLSO.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSO/EllipseHalfUniformLSO.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSO/EllipseWholeUniformLSO.png" alt="c" width="300" height="300"/>

Точки разбросаны нормально:

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSO/EllipseQuarterNormLSO.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSO/EllipseHalfNormLSO.png" alt="c" width="300" height="300"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/cde9f260ff239605cf492b27bc155f97d2716f05/Curve-Fitting/images/contour/ellipse/LSO/EllipseWholeNormLSO.png" alt="c" width="300" height="300"/>

