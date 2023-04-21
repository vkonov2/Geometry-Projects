<h1 align="left">Segment Reconstruction From Plane Projections</h1>

<h2>Origin of the problem</h2>

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b1.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b2.png" alt="c" width="150" height="150"/>

<h2 align="left">Problem Statement</h2>

There are severeal sets of points on several planes in space. It is necessary to reconstruct the segment which is a prototype of projections on the planes as accurately as possible.



<h2 align="left">Method №1</h2>

<h3 align="left">Idea</h3>

1) Approximate the sets of points by lines.
2) Find the planes through the obtained straight lines and projection directions and obtain the prism in the intersection of the three planes.
3) Find the line minimally distant from the three given planes.




<h3 align="left">Examples</h3>



<h2 align="left">Method №2</h2>

<h3 align="left">Idea</h3>

1) Create a grid with all z-coordinate values of all points from the three sets.
2) In each set add points with z that are present in the grid but absent in the current set: these points are added on the segments connecting the two nearest points of the set. In this way a layering of points is obtained.
3) In each layer find lines from points parallel to the projection directions and obtain a triangle. Find the point minimally distant from the three given lines.
4) Approximate the obtained set by the straight line.



<h3 align="left">Examples</h3>



<h2 align="left">Comparison of methods and verification of results</h2>

Let we find the segment (straight line). The idea is to project this line onto the plane of points and find the sum of the squared distances from the points to the projections. Comparison of the values of this functional will show the level of deviation of the obtained straight line from the original one or the level of error of the method.

$$\Lambda = \underset{j=1}{\overset{3}{\sum}}\underset{i=1}{\overset{n_1, n_2, n_3}{\sum}}(A_j x_i^j + B_j y_i^j + C_j z_i^j + D_j)^2$$

\begin{tabular}{|c|c|c|}
	\hline
	 & \textbf{Method №1} & \textbf{Method №2}\\
	\hline
	synthetic data & 5.286735e-3 & 6.596934e-3 \\
	\hline
	real data (positive) & 2.143562e-6 & 6.543256e-6 \\
	\hline
	real data (negative) & 5.324546e-4 & 8.859035e-4 \\
	\hline
\end{tabular}

<h2 align="left">Conclusion</h2>

In the work, I considered several methods for reconstructing a segment from its projections on planes and compared them on synthetic and real data. These methods do not achieve the desired reconstruction accuracy, so further research will be directed to the development of other methods that provide better accuracy.

<h2 align="left">References</h2>

1) Preparata F., Sheimos M. Computational geometry: Introduction. --- M.: Mir, 1989. --- 478 p.
2) Veselov A.P., Troitsky E.V. Lectures on analytic geometry. --- M.: Publishing House of the Center for Applied Research at the Faculty of Mechanics and Mathematics of Moscow State University. 2002. --- 160 p.
