
#include "../MeshFunctions.h"

int main(int argc, char** argv)
{
	Polyhedron_3 	ph_ori, 	ph_oct,		ph_cgl;
	string 			flnm_ori, 	flnm_oct,	flnm_cgl;
	double time = 0.;
	vector<double> rs_oct, rs_cgl;

	flnm_ori 	= "models/PolyOri.off";
	flnm_oct	= "models/PolyOct.off";

	if(!CGAL::IO::read_polygon_mesh(flnm_ori, ph_ori) || !CGAL::is_triangle_mesh(ph_ori))
	{
		cerr << "Invalid mesh in file --> " << flnm_ori << endl;
		return EXIT_FAILURE;
	}

	if(!CGAL::IO::read_polygon_mesh(flnm_oct, ph_oct) || !CGAL::is_triangle_mesh(ph_oct))
	{
		cerr << "Invalid mesh in file --> " << flnm_oct << endl;
		return EXIT_FAILURE;
	}

	cout << "\npoints in " << flnm_ori << ":\t" << ph_ori.size_of_vertices() << endl;
	cout << "points in " << flnm_oct << ":\t" << ph_oct.size_of_vertices() << endl;

	FindMetrics(ph_ori, ph_oct, rs_oct, time);

	cout << scientific;

	cout << "\nOctonus --> " << flnm_oct << endl;
	cout << "residual = " << rs_oct[0] << endl;
	cout << "mean     = " << rs_oct[1] << endl;
	cout << "std      = " << rs_oct[2] << endl;
	
	for (int i = 1; i < 21; i++)
	{
		flnm_cgl = "models/Poly" + to_string(i) + ".off";

		if(!CGAL::IO::read_polygon_mesh(flnm_cgl, ph_cgl) || !CGAL::is_triangle_mesh(ph_cgl))
		{
			cerr << "Invalid mesh in file --> " << flnm_cgl << endl;
			return EXIT_FAILURE;
		}

		cout << "\npoints in " << flnm_cgl << ":\t" << ph_cgl.size_of_vertices() << endl;
		cout << "points in " << flnm_oct << ":\t" << ph_oct.size_of_vertices() << endl;

		FindMetrics(ph_ori, ph_cgl, rs_cgl, time);

		cout << "           CGAL " << i << "\t\tOctonus" << endl;
		cout << "residual = " << rs_cgl[0] << " <===> " << rs_oct[0] << endl;
		cout << "mean     = " << rs_cgl[1] << " <===> " << rs_oct[1] << endl;
		cout << "std      = " << rs_cgl[2] << " <===> " << rs_oct[2] << endl;

		rs_cgl.clear();
		ph_cgl.clear();
	}

	cout.unsetf(ios::scientific);

	cout << "\n" << time << " secs" << endl;

	rs_oct.clear();
	ph_ori.clear();
	ph_oct.clear();

	return EXIT_SUCCESS;
}