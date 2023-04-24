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

<!-- <img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b1.png" alt="c" width="150" height="150"/> -->
