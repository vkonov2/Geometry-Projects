<h1 align="left">Построение прямой, аппроксимирующей множество точек</h1>

<h3 align="left">Постановка задачи</h3>

Дано множество из точек в плоскости или пространстве. Необходимо построить прямую, максимально близкую к заданному множетсву. Меру близости выберем равной корню из суммы квадратов расстояний от точек множества до прямой. Соответственно, необходимо подобрать такие коэффициенты прямой, чтобы для данного набора точек мера на данном множестве и прямой с этими коэффициентами была минимальна.

<img src="https://github.com/vkonov2/Geometry-Projects/blob/f0d3c3b501848cacb86ef4eb328352338f184da4/Preliminary-Algorithms/Least-Square-Line-Fitting/images/1.jpg" alt="c" width="260" height="260"/> <img src="https://github.com/vkonov2/Geometry-Projects/blob/f0d3c3b501848cacb86ef4eb328352338f184da4/Preliminary-Algorithms/Least-Square-Line-Fitting/images/2.jpg" alt="c" width="300" height="260"/>

<h2 align="left">Двумерный случай</h2>

Пусть имеется набор из $n$ точек: $P = \Set{(x_i, y_i), i=1,\dots,n}$. 

Будем искать прямую в виде $a \cdot x + b \cdot y + d = 0$, где $a, b, d$ - неизвестные коэффициенты.

По определению расстояние от точки $(x_0, y_0)$ до прямой $ax+by+d=0$ определяется по следующей формуле:

$$\rho (ax+by+d=0, (x_0, y_0)) = \frac{|ax_0+by_0+d|}{\sqrt{a^2+b^2}}$$

Тогда наша мера будет иметь вид:

$$\Lambda (ax+by+d=0,P)=\sqrt{\sum_{i=1}^{n} \frac{(ax_i+by_i+d)^2}{a^2+b^2}}$$

Будем искать нормированное уравнение прямой, которое получается из общего уравнения делением всех членов на $\sqrt{a^2+b^2}$: $a^{'}x+b^{'}y+d^{'}=0$. Тогда расстояние от точки $(x_0,y_0)$ до прямой равно абсолютному значению отклонения и вычисляется по формуле: 

$$\rho (a^{'}x+b^{'}y+d^{'}=0, (x_0, y_0)) = |a^{'}x_0+b^{'}y_0+d^{'}|$$

Тогда наша мера будет иметь вид:

$$\Lambda (a^{'}x+b^{'}y+d^{'}=0,P)=\sqrt{\sum_{i=1}^{n} (a^{'}x_i+b^{'}y_i+d^{'})^2}, (a^{'})^2+(b^{'})^2=1$$

Будем исследовать не саму меру, а подкоренное выражение, и для простоты будем рассматривать коэффициенты без индексов.

Таким образом, задача свелась к поиску минимального значения выражения $\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+d)^2$ с ограничением $a^2+b^2=1$.

Будем искать коэффициенты методом Лагранжа. Составим функцию Лагранжа:

$$L(a, b, d, \lambda) = \underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+d)^2 - \lambda (a^2+b^2-1)$$

Составим систему из четырех уравнений, приравняв к нулю частные производные функции Лагранжа $L(a, b, d, \lambda)$ по $a, b, d$ и $\lambda$:

$$\begin{cases}
		\frac{\partial}{\partial a} (\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+d)^2 - \lambda (a^2+b^2-1)) = 0 \\
		\frac{\partial}{\partial b} (\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+d)^2 - \lambda (a^2+b^2-1)) = 0 \\
		\frac{\partial}{\partial d} (\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+d)^2 - \lambda (a^2+b^2-1)) = 0 \\
		\frac{\partial}{\partial \lambda} (\underset{i=1}{\overset{n}{\sum}}(ax_i+by_i+d)^2 - \lambda (a^2+b^2-1)) = 0
	\end{cases}$$

Таким образом, задача свелась к поиску собственных значений матрицы $A$:

$$A = \begin{pmatrix}
		\underset{i=1}{\overset{n}{\sum}}x_i^2-\frac{1}{n}\underset{i=1}{(\overset{n}{\sum}} x_i)^2
		& 
		\underset{i=1}{\overset{n}{\sum}}x_i y_i - \frac{1}{n}\underset{i=1}{\overset{n}{\sum}}x_i \underset{i=1}{\overset{n}{\sum}} y_i
		\\
		\underset{i=1}{\overset{n}{\sum}}x_i y_i - \frac{1}{n}\underset{i=1}{\overset{n}{\sum}}x_i \underset{i=1}{\overset{n}{\sum}} y_i
		&
		\underset{i=1}{\overset{n}{\sum}}y_i^2-\frac{1}{n}\underset{i=1}{(\overset{n}{\sum}} y_i)^2
	\end{pmatrix}$$

Тем или иным способом, найдем собственные значения. В программе удобно пользоваться методом Якоби.

$$\lambda_1 = \frac{A_{1,1}+A_{2,2} + \sqrt{(A_{1,1}+A_{2,2})^2-4 (A_{1,1} A_{2,2} - A_{1,2} A_{2,1})}}{2}$$

$$\lambda_2 = \frac{A_{1,1}+A_{2,2} - \sqrt{(A_{1,1}+A_{2,2})^2-4 (A_{1,1} A_{2,2} - A_{1,2} A_{2,1})}}{2}$$

Получаем две системы:

$$\begin{cases}
		(A_{1,1} - \lambda_i) a + A_{1,2} b = 0 \\
		A_{2,1} a + (A_{2,2} - \lambda_i) b = 0
	\end{cases},i=1,2$$

Данная система однородная, поэтому имеет бесконечное число решений.

Найдем теперь собтвенные векторы. Выразим $a$ через $b$, например, из первых уравнений последних систем:

$$a = -\frac{A_{1,2}}{A_{1,1}-\lambda_i}b,i=1,2$$

Положим $b = 1$, тогда $a_i = -\frac{A_{1,2}}{A_{1,1}-\lambda_i},\;i=1,2$.

Уравнение для $d$ через $a$ и $b$ получаем следующее:

$$d_i = \frac{1}{n}(a_i * \underset{j=1}{\overset{n}{\sum}}x_j + \underset{j=1}{\overset{n}{\sum}}y_j),i=1,2$$

Далее узнаем значение нашего функционала при данных значениях $a_i, b_i, d$:

$$L_i (a_i, b = 1, d_i)= \underset{j=1}{\overset{n}{\sum}}(a_i x_j+ y_j + d_i)^2,i=1,2$$

Выбираем $L = min \{L_1, L_2\}$. Без ограничения общности пусть $L_1 < L_2 \; \Rightarrow \; a = a_1, b = 1, d = d_1$.

Таким образом, итоговые формулы для коэффициентов прмяой получаются следующие:

$$a = -\frac{A_{1,2}}{A_{1,1}-\lambda_1}, \; b =1, \; d = -\frac{1}{n}(a * \underset{j=1}{\overset{n}{\sum}}x_j + \underset{j=1}{\overset{n}{\sum}}y_j)$$

<h2 align="left">Трехмерный случай</h2>

Пусть имеется набор из $n$ точек: $P = \Set{(x_i, y_i, z_i), i=1, \dots,n}$. 

Будем искать прямую в параметрическом виде. Пусть $(m, n, p)$ - напрявляющий вектор прямой, $(x_0, y_0, z_0)$ - некоторая точка прямой. Тогда следующая система задает прямую в пространстве: 

$$\begin{cases}
		x = x_0 + m \cdot t \\
		y = y_0 + n \cdot t \\
		z = z_0 + p \cdot t
	\end{cases}$$

В качестве точки $(x_0, y_0, z_0)$ выбирем центр масс системы данных точек. Таким образом:
$x_0 = \frac{1}{n}\underset{i=1}{\overset{n}{\sum}} x_i, \; y_0 = \frac{1}{n}\underset{i=1}{\overset{n}{\sum}} y_i, \; z_0 = \frac{1}{n}\underset{i=1}{\overset{n}{\sum}} z_i$.

По определению расстояние от точки $M_1$ до прямой $\mathit{l}$ определяется по следующей формуле:

$M_0 (x_0, y_0, z_0)$ - точка на прямой $\mathit{l}$, $\mathit{l} = (m, n, p)$ - напрявляющий вектор прямой, $d (M_1, \mathit{l}) = \frac{|[\overline{\mathit{l}}, \overline{M_0 M_1}]|}{|\overline{\mathit{l}}|}$

<img src="https://github.com/vkonov2/Geometry-Projects/blob/f0d3c3b501848cacb86ef4eb328352338f184da4/Preliminary-Algorithms/Least-Square-Line-Fitting/images/4.jpg" alt="c" width="460" height="200"/>

Распишем составляющие элементы формулы для расстояния:

$|\overline{l}| = \sqrt{m^2 + n^2 + p^2}$

$\overline{M_0 M_1} = (x_1 - x_0, y_1 - y_0, z_1 - z_0)$

$$[\overline{\mathit{l}}, \overline{M_0 M_1}] = \begin{vmatrix}
	\mathbf{i} & \mathbf{j} & \mathbf{k} \\
	m & n & p \\
	x_1 - x_0 & y_1 - y_0 & z_1 - z_0
\end{vmatrix} = \left(\begin{vmatrix}
	n & p \\
	y_1 - y_0 & z_1 - z_0
\end{vmatrix}, -\begin{vmatrix}
	m & p \\
	x_1 - x_0 & z_1 - z_0
\end{vmatrix}, \begin{vmatrix}
	m & n \\
	x_1 - x_0 & y_1 - y_0
\end{vmatrix}\right) = $$

$$ = (n (z_1 - z_0) - p (y_1 - y_0), p (x_1 - x_0) - m (z_1 - z_0), m (y_1 - y_0) - n (x_1 - x_0)) = $$

$$ = (n \triangle_{z_1} - p \triangle_{y_1}, p \triangle_{x_1} - m \triangle_{z_1}, m \triangle_{y_1} - n \triangle_{x_1})\text{, где }\begin{cases}
	\triangle_{x_1} = x_1 - x_0 \\
	\triangle_{y_1} = y_1 - y_0 \\
	\triangle_{z_1} = z_1 - z_0
\end{cases}$$

$|[\overline{\mathit{l}}, \overline{M_0 M_1}]| = \sqrt{(n \triangle_{z_1} - p \triangle_{y_1})^2 + (p \triangle_{x_1} - m \triangle_{z_1})^2 + (m \triangle_{y_1} - n \triangle_{x_1})^2}$

Тогда формула расстояния принимает вид:

$$d (M_1, \mathit{l}) = \sqrt{\frac{(n \triangle_{z_1} - p \triangle_{y_1})^2 + (p \triangle_{x_1} - m \triangle_{z_1})^2 + (m \triangle_{y_1} - n \triangle_{x_1})^2}{m^2 + n^2 + p^2}}$$

Тогда наша мера примет вид:

$$\Lambda (\mathit{l}, P) = \sqrt{\underset{i=1}{\overset{n}{\sum}} \frac{(n \triangle_{z_i} - p \triangle_{y_i})^2 + (p \triangle_{x_i} - m \triangle_{z_i})^2 + (m \triangle_{y_i} - n \triangle_{x_i})^2}{m^2 + n^2 + p^2} } = $$

$$ = \frac{1}{\sqrt{m^2 + n^2 + p^2}} \sqrt{\underset{i=1}{\overset{n}{\sum}} \left( (n \triangle_{z_i} - p \triangle_{y_i})^2 + (p \triangle_{x_i} - m \triangle_{z_i})^2 + (m \triangle_{y_i} - n \triangle_{x_i})^2 \right)}$$

Будем искать нормированное уравнение прямой, которое получается из общего уравнения делением всех членов на $\sqrt{m^2 + n^2 + p^2}$. Тогда расстояние от точки $(x_0,y_0,z_0)$ до прямой равно абсолютному значению векторного произведения и вычисляется по формуле: 

$$d (M_1, \mathit{l}) = \sqrt{(n \triangle_{z_1} - p \triangle_{y_1})^2 + (p \triangle_{x_1} - m \triangle_{z_1})^2 + (m \triangle_{y_1} - n \triangle_{x_1})^2}$$

Тогда наша мера будет иметь вид:

$$\Lambda (\mathit{l}, P) = \sqrt{\underset{i=1}{\overset{n}{\sum}} \left((n \triangle_{z_i} - p \triangle_{y_i})^2 + (p \triangle_{x_i} - m \triangle_{z_i})^2 + (m \triangle_{y_i} - n \triangle_{x_i})^2 \right)}$$

Будем исследовать не саму меру, а подкоренное выражение.

Таким образом, задача свелась к поиску минимального значения выражения $\underset{i=1}{\overset{n}{\sum}} \left((n \triangle_{z_i} - p \triangle_{y_i})^2 + (p \triangle_{x_i} - m \triangle_{z_i})^2 + (m \triangle_{y_i} - n \triangle_{x_i})^2 \right)$ с ограничением $m^2 + n^2 + p^2=1$.

Будем искать коэффициенты методом Лагранжа. Составим функцию Лагранжа:

$$L(m, n, p, \lambda) = \underset{i=1}{\overset{n}{\sum}} \left((n \triangle_{z_i} - p \triangle_{y_i})^2 + (p \triangle_{x_i} - m \triangle_{z_i})^2 + (m \triangle_{y_i} - n \triangle_{x_i})^2 \right) -$$

$$- \lambda (m^2 + n^2 + p^2-1)$$

Для простоты чтения обозначим коэффициенты в данной формуле следующим образом:

$$\begin{cases}
		\underset{i=1}{\overset{n}{\sum}} \left((\triangle_{x_i})^2 + (\triangle_{z_i})^2\right) = a_{xz} \\
		\underset{i=1}{\overset{n}{\sum}} \left((\triangle_{y_i})^2 + (\triangle_{z_i})^2\right) = a_{yz} \\
		\underset{i=1}{\overset{n}{\sum}}\left((\triangle_{x_i})^2 + (\triangle_{y_i})^2\right) = a_{xy} \\
		\underset{i=1}{\overset{n}{\sum}} \triangle_{y_i} \triangle_{z_i} = b_{yz} \\
		\underset{i=1}{\overset{n}{\sum}} \triangle_{x_i} \triangle_{z_i} = b_{xz} \\
		\underset{i=1}{\overset{n}{\sum}} \triangle_{x_i} \triangle_{y_i} = b_{xy}
	\end{cases}$$

Таким образом, получаем следующую функцию Лагранжа:

$$L(m, n, p, \lambda) = a_{xz} n^2 + a_{yz} m^2 + a_{xy} p^2 - 2 b_{yz} n p - 2 b_{xz} m p  - 2 b_{xy} m n - \lambda (m^2 + n^2 + p^2-1)$$

Составим систему из четырех уравнений, приравняв к нулю частные производные функции Лагранжа $L(m, n, p, \lambda)$ по $m, n, p$ и $\lambda$:

$$\begin{cases}
		\frac{\partial}{\partial m} \left(a_{xz} n^2 + a_{yz} m^2 + a_{xy} p^2 - 2 b_{yz} n p - 2 b_{xz} m p  - 2 b_{xy} m n - \lambda (m^2 + n^2 + p^2-1)\right) = 0 \\
		\frac{\partial}{\partial n} \left(a_{xz} n^2 + a_{yz} m^2 + a_{xy} p^2 - 2 b_{yz} n p - 2 b_{xz} m p  - 2 b_{xy} m n - \lambda (m^2 + n^2 + p^2-1)\right) = 0 \\
		\frac{\partial}{\partial p} \left(a_{xz} n^2 + a_{yz} m^2 + a_{xy} p^2 - 2 b_{yz} n p - 2 b_{xz} m p  - 2 b_{xy} m n - \lambda (m^2 + n^2 + p^2-1)\right) = 0 \\
		\frac{\partial}{\partial \lambda} \left(a_{xz} n^2 + a_{yz} m^2 + a_{xy} p^2 - 2 b_{yz} n p - 2 b_{xz} m p  - 2 b_{xy} m n - \lambda (m^2 + n^2 + p^2-1)\right) = 0
	\end{cases}$$

Решение первых трех уравнений эквивалентно решению матричного уравнения:

$$\begin{pmatrix}
	a_{yz} - \lambda & -b_{xy} & -b_{xz} \\
	-b_{xy} & a_{xz} - \lambda & -b_{yz} \\
	-b_{xz} & -b_{yz} & a_{xy} - \lambda
\end{pmatrix}\cdot \begin{pmatrix}
	m \\ n \\ p
\end{pmatrix} = 0$$

$$(A - \lambda E)\cdot \begin{pmatrix}
	m \\ n \\ p
\end{pmatrix} = 0,\text{ где } A = \begin{pmatrix}
	a_{yz}  & -b_{xy} & -b_{xz} \\
	-b_{xy} & a_{xz} & -b_{yz} \\
	-b_{xz} & -b_{yz} & a_{xy}
\end{pmatrix}, E = \begin{pmatrix}
	1 & 0 & 0 \\
	0 & 1 & 0 \\
	0 & 0 & 1
\end{pmatrix}$$

Тогда задача свелась к поиску собственных значений для матрицы $A$. Тем или иным способом (например, методом Якоби) находим три собственных значения: $\lambda_i, i=1,2,3$.

Получаем три системы:

$$\begin{cases}
		(a_{yz} - \lambda_i ) m + (-b_{xy}) n + (-b_{xz}) p = 0 \\
		(-b_{xy})m + (a_{xz} - \lambda_i)n + (-b_{yz})p = 0 \\
		(-b_{xz})m + (-b_{yz})n + (a_{xy} - \lambda_i)p = 0
	\end{cases}, i = 1,2,3$$

Данная система однородная, поэтому имеет бесконечное число решений.

Найдем теперь собтвенные векторы. Выразим $m$ через $n$ и $p$, например, из первых уравнений систем:

$$m = \frac{b_{xy} n + b_{xz} p }{a_{yz} - \lambda_i}, i = 1,2,3$$

Подставим данное выражение во второе и третье уравнения систем:

$$\begin{cases}
	(- \frac{b_{xy}^2}{a_{yz} - \lambda_i} + a_{xz} - \lambda_i) n + (-\frac{b_{xy} b_{xz}}{a_{yz} - \lambda_i} - b_{yz})p = 0 \\
	(-\frac{b_{xz} b_{xy}}{a_{yz} - \lambda_i} - b_{yz}) n + (- \frac{b_{xz}^2}{a_{yz} - \lambda_i} + a_{xy} - \lambda_i) p = 0
\end{cases}, i = 1,2,3$$

Выразим $n$ через $p$ из первого уравнения текущей системы:

$$n = \frac{\frac{b_{xy} b_{xz}}{a_{yz} - \lambda_i} + b_{yz}}{- \frac{b_{xy}^2}{a_{yz} - \lambda_i} + a_{xz} - \lambda_i} p, i=1,2,3$$

Положим $p = 1$. Тогда получаем следующие выражения:

$$n = \frac{\frac{b_{xy} b_{xz}}{a_{yz} - \lambda_i} + b_{yz}}{- \frac{b_{xy}^2}{a_{yz} - \lambda_i} + a_{xz} - \lambda_i}, i =1,2,3$$

$$m = \frac{b_{xy} \left(\frac{\frac{b_{xy} b_{xz}}{a_{yz} - \lambda_i} + b_{yz}}{- \frac{b_{xy}^2}{a_{yz} - \lambda_i} + a_{xz} - \lambda_i}\right) + b_{xz}}{a_{yz} - \lambda_i}, i = 1,2,3$$

Далее узнаем значение нашего функционала при данных значениях $m_i, n_i, p$:

$$L_i (m_i, n_i, p = 1)= a_{xz} n_i^2 + a_{yz} m_i^2 + a_{xy} - 2 b_{yz} n_i - 2 b_{xz} m_i  - 2 b_{xy} m_i n_i, i=1,2,3$$

Выбираем $L = min \{L_1, L_2, L_3\}$. Без ограничения общности пусть $L_1 < L_2 < L_3 \; \Rightarrow \; m = m_1, n = n_1, p = 1$.

Таким образом, итоговые формулы для коэффициентов прмяой получаются следующие:

$$\begin{cases}
	m = \frac{b_{xy} \left(\frac{\frac{b_{xy} b_{xz}}{a_{yz} - \lambda_1} + b_{yz}}{- \frac{b_{xy}^2}{a_{yz} - \lambda_1} + a_{xz} - \lambda_1}\right) + b_{xz}}{a_{yz} - \lambda_1} \\
	n = \frac{\frac{b_{xy} b_{xz}}{a_{yz} - \lambda_1} + b_{yz}}{- \frac{b_{xy}^2}{a_{yz} - \lambda_1} + a_{xz} - \lambda_1} \\
	p = 1 \\
	(x_0, y_0, z_0)\text{ - центр масс системы:} \\
	x_0 = \frac{1}{n}\underset{i=1}{\overset{n}{\sum}} x_i, \; y_0 = \frac{1}{n}\underset{i=1}{\overset{n}{\sum}} y_i, \; z_0 = \frac{1}{n}\underset{i=1}{\overset{n}{\sum}} z_i
\end{cases}$$

<h2 align="left">Примеры работы</h2>

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/f0d3c3b501848cacb86ef4eb328352338f184da4/Preliminary-Algorithms/Least-Square-Line-Fitting/images/6.jpg" alt="c" width="400" height="400"/> <img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/f0d3c3b501848cacb86ef4eb328352338f184da4/Preliminary-Algorithms/Least-Square-Line-Fitting/images/8.jpg" alt="c" width="400" height="400"/>
