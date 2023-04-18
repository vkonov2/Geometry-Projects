
#include "../MeshFunctions.h"

int main (int argc, char** argv)
{	
	double time = 0.;

	string fname_off = "../../../models/PolyOriginal.off";
	string fname_pwn = "../../../models/Poly.pwn";

	list<PointVectorPair> pts;

    ReadPtVcPairOff(fname_off, pts, time);

    EstimateNormals(pts, 0, time);

    WritePtVcPairPwn(fname_pwn, pts, time);

    pts.clear();

    return EXIT_SUCCESS;

	Pwn_vector points;

	if(!CGAL::IO::read_points(fname_pwn, back_inserter(points),
	                        CGAL::parameters::point_map(Point_map())
	                                         .normal_map(Normal_map())))
	{
		cerr << "Error: cannot read file cube.pwn" << endl;
		return EXIT_FAILURE;
	}

	cerr << points.size() << " point(s) read." << endl;

	Efficient_ransac ransac;
	ransac.set_input(points);
	ransac.add_shape_factory<Plane>();
	ransac.detect();
	Efficient_ransac::Plane_range planes = ransac.planes();
	Pwn_vector structured_pts;

	CGAL::structure_point_set(points,
	                        planes,
	                        back_inserter(structured_pts),
	                        5e-3, // epsilon for structuring points
	                        CGAL::parameters::point_map(Point_map())
	                                         .normal_map(Normal_map())
	                                         .plane_map(CGAL::Shape_detection::Plane_map<Traits>())
	                                         .plane_index_map(CGAL::Shape_detection::Point_to_shape_index_map<Traits>(points, planes)));
	cerr << structured_pts.size ()
	        << " structured point(s) generated." << endl;
	CGAL::IO::write_points("out.pwn", structured_pts,
	                     CGAL::parameters::point_map(Point_map())
	                                      // .normal_map(Normal_map())
	                                      .stream_precision(17));
	return EXIT_SUCCESS;
}