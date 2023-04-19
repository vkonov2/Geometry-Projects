<h1 align="left">Построение прямой, аппроксимирующей множество точек</h1>

<h3 align="left">Постановка задачи</h3>

Дано множество из точек в плоскости или пространстве. Необходимо построить прямую, максимально близкую к заданному множетсву. Меру близости выберем равной корню из суммы квадратов расстояний от точек множества до прямой. Соответственно, необходимо подобрать такие коэффициенты прямой, чтобы для данного набора точек мера на данном множестве и прямой с этими коэффициентами была минимальна.

<img src="https://github.com/vkonov2/Geometry-Projects/blob/f0d3c3b501848cacb86ef4eb328352338f184da4/Preliminary-Algorithms/Least-Square-Line-Fitting/images/1.jpg" alt="c" width="260" height="260"/> <img src="https://github.com/vkonov2/Geometry-Projects/blob/f0d3c3b501848cacb86ef4eb328352338f184da4/Preliminary-Algorithms/Least-Square-Line-Fitting/images/2.jpg" alt="c" width="260" height="260"/>

<h3 align="left">Двумерный случай</h3>

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
