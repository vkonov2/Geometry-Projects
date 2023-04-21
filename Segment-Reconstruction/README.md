<h1 align="left">Segment Reconstruction From Plane Projections</h1>

<h2>Origin of the problem</h2>

It is necessary to say a few words about the origin of this problem. This task is an actual production problem that can be encountered in the gemstone processing. For an accurate processing of precious stones, it is necessary to create their exact solid model at each stage of processing. For this purpose, the stone parameters should be found using different technologies.

Here you see the x-ray scan data of the emerald. These data show the density of the stone material at each point in space.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b1.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b2.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b3.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b4.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b5.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b6.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b7.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b8.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b9.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b10.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b11.png" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/b12.png" alt="c" width="150" height="150"/>

Here you see the photometric data of the emerald. These data show the reflective and refractive properties of the stone, from which it is possible to make a conclusion about the shape of the emerald and about the relative arrangement of faces, edges and vertices.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s1.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s2.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s3.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s4.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s5.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s6.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s7.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s8.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s9.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s10.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s11.jpg" alt="c" width="150" height="150"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/s12.jpg" alt="c" width="150" height="150"/>

Anyway, we have a set of projection contours on the planes from which measurements were taken. Now, according to these contours (that is, according to the coordinates of points on the projection planes and along the directions of these projections), it is necessary to reconstruct coordinates of the vertices of the original stone and to find its edges and faces with great accuracy.

<h2 align="left">Problem Statement</h2>

Now let’s turn to the theoretical statement of the problem. There are several sets of points on several planes in space. It is necessary to reconstruct the segment which is a prototype of projections on the planes as accurately as possible. Here you can see an example the problem with three point sets and three planes of projections.

Three point sets, three planes and a segment.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/10.jpg" alt="c" width="400" height="400"/>

<h2 align="left">Method №1</h2>

<h3 align="left">Idea</h3>

1) Approximate the sets of points by lines.
2) Find the planes through the obtained straight lines and projection directions and obtain the prism in the intersection of the three planes.
3) Find the line minimally distant from the three given planes.

Construction of lines and planes through points of sets.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/11.jpg" alt="c" width="400" height="400"/>

<h3 align="left">Example (synthetic)</h3>

Found segment and initial sets of synthetic data.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth1synth.png" alt="c" width="400" height="400"/>

Projections of the resulting segment on the planes.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth1synthproec1.png" alt="c" width="200" height="200"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth1synthproec2.png" alt="c" width="200" height="200"/><img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth1synthproec3.png" alt="c" width="200" height="200"/>

<h3 align="left">Example (real)</h3>

Straight lines that approximate the point sets.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth1lineapprox.png" alt="c" width="500" height="400"/>

Found segments, spatial solid and original projection contours.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth1realresult.png" alt="c" width="500" height="400"/> <img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth1realsideview.png" alt="c" width="500" height="400"/>

<h2 align="left">Method №2</h2>

<h3 align="left">Idea</h3>

1) Create a grid with all z-coordinate values of all points from the three sets.
2) In each set add points with z that are present in the grid but absent in the current set: these points are added on the segments connecting the two nearest points of the set. In this way a layering of points is obtained.
3) In each layer find lines from points parallel to the projection directions and obtain a triangle. Find the point minimally distant from the three given lines.
4) Approximate the obtained set by the straight line.

Adding points and layering.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/120.jpg" alt="c" width="400" height="400"/>

Obtaining the point minimally distant from the three given lines.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/122.jpg" alt="c" width="550" height="400"/>

<h3 align="left">Example (synthetic)</h3>

Average points in each layer and the initial sets.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth2synthmidpoints.png" alt="c" width="500" height="400"/>

Found segment and initial sets of synthetic data.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth2synth.png" alt="c" width="500" height="400"/>

<h3 align="left">Example (real)</h3>

Found segments, spatial solid and original projection contours.

<img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth2real.png" alt="c" width="500" height="400"/> <img align="center" src="https://github.com/vkonov2/Geometry-Projects/blob/630232146f9498147a11eb3fc29c8c9fd458934a/Segment-Reconstruction/images/meth2realsideview.png" alt="c" width="500" height="400"/>

<h2 align="left">Comparison of methods and verification of results</h2>

Let we find the segment (straight line). The idea is to project this line onto the plane of points and find the sum of the squared distances from the points to the projections. Comparison of the values of this functional will show the level of deviation of the obtained straight line from the original one or the level of error of the method.

$$\Lambda = \underset{j=1}{\overset{3}{\sum}}\underset{i=1}{\overset{n_1, n_2, n_3}{\sum}}(A_j x_i^j + B_j y_i^j + C_j z_i^j + D_j)^2$$

|   					|Method №1 		|Method №2	|
|:----------------------|:-------------:|:---------:|
|synthetic data   		|5.286735e-3   	|6.596934e-3|
|real data (positive)   |2.143562e-6   	|6.543256e-6|
|real data (positive)   |5.324546e-4   	|8.859035e-4|

<h2 align="left">Conclusion</h2>

In the work, I considered several methods for reconstructing a segment from its projections on planes and compared them on synthetic and real data. These methods do not achieve the desired reconstruction accuracy, so further research will be directed to the development of other methods that provide better accuracy.

<h2 align="left">References</h2>

1) Preparata F., Sheimos M. Computational geometry: Introduction. --- M.: Mir, 1989. --- 478 p.
2) Veselov A.P., Troitsky E.V. Lectures on analytic geometry. --- M.: Publishing House of the Center for Applied Research at the Faculty of Mechanics and Mathematics of Moscow State University. 2002. --- 160 p.
