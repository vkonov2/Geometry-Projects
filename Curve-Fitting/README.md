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


