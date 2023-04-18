
#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>

#include <boost/lexical_cast.hpp>

#include <CGAL/IO/STL.h>
#include <CGAL/IO/OBJ.h>
  
#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/boost/graph/graph_traits_Surface_mesh.h>

#include <CGAL/Surface_mesh_approximation/approximate_triangle_mesh.h>

typedef CGAL::Simple_cartesian<double>               Kernel;
typedef Kernel::Point_3                              Point_3;
typedef std::vector<std::size_t>                     Polygon_3;
typedef CGAL::Surface_mesh<Point_3>                  Surface_mesh;

namespace PMP = CGAL::Polygon_mesh_processing;
namespace VSA = CGAL::Surface_mesh_approximation;


namespace params = CGAL::parameters;

int main(int argc, char** argv)
{

	std::string folder = (argc > 1) ? argv[1] : "Cube";
	int num = (argc > 2) ? atoi(argv[2]) : 6;

	std::string path = "../../models/originals/" + folder + "/";
	std::string file = "Cube";

	std::string filename = path+file+std::to_string(num)+".stl";

	Surface_mesh surface_mesh;

	std::ifstream is(filename);

	std::vector<Point_3> points_ref;
	std::vector<Polygon_3> faces_ref;

	if(!is || !(CGAL::IO::read_STL(is, points_ref, faces_ref)))
	{
		std::cerr << "Failed to read input mesh: " << filename << std::endl;
		return EXIT_FAILURE;
	}

	PMP::orient_polygon_soup(points_ref, faces_ref);
	PMP::polygon_soup_to_polygon_mesh(points_ref, faces_ref, surface_mesh);

	if(!CGAL::is_triangle_mesh(surface_mesh))
	{
		std::cerr << "Input geometry is not triangulated." << std::endl;
		return EXIT_FAILURE;
	}

	std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

	std::vector<Kernel::Point_3> anchors;
  	std::vector<std::array<std::size_t, 3> > triangles;

	bool is_manifold = VSA::approximate_triangle_mesh(surface_mesh, CGAL::parameters::verbose_level(VSA::MAIN_STEPS).max_number_of_proxies((int)(faces_ref.size())).anchors(std::back_inserter(anchors)).triangles(std::back_inserter(triangles)));

	std::chrono::steady_clock::time_point end_time = std::chrono::steady_clock::now();

	int val = (argc > 3) ? atoi(argv[3]) : 0;
	bool flg = (argc > 3 && val == 1) ? true : false;
	bool flag = (argc > 3) ? flg : is_manifold;

	if (flag) 
	{
	    std::cout << "oriented, 2-manifold output." << std::endl;
	    PMP::orient_polygon_soup(anchors, triangles);
	    Surface_mesh output;
	    PMP::polygon_soup_to_polygon_mesh(anchors, triangles, output);
	    if (CGAL::is_closed(output) && (!PMP::is_outward_oriented(output)))
	      PMP::reverse_face_orientations(output);

	  	std::cout << "#anchor points: " << anchors.size() << std::endl;
  		std::cout << "#triangles: " << triangles.size() << std::endl;

	    CGAL::IO::write_polygon_mesh((argc > 4) ? argv[4] : file+"Approximated.stl", output, CGAL::parameters::stream_precision(17));

	    std::string command = "open -a /Applications/MeshLab2022.02.app "+file+"Approximated.stl";

	    system(command.c_str());
	 }

	std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()/1e+3 << "s" << std::endl;

	return EXIT_SUCCESS;
}