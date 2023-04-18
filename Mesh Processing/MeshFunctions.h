
#include <stdio.h>

#include <vector>
#include <string>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <tuple>
#include <utility>
#include <cstdlib>
#include <iterator>
#include <filesystem>

#include <boost/lexical_cast.hpp>

#include <CGAL/IO/STL.h>
#include <CGAL/IO/OBJ.h>
#include <CGAL/IO/OFF.h>
#include <CGAL/IO/read_xyz_points.h>
#include <CGAL/IO/write_xyz_points.h>
#include <CGAL/IO/read_ply_points.h>
#include <CGAL/IO/read_points.h>
#include <CGAL/IO/write_points.h>
#include <CGAL/IO/Color.h>

#include <CGAL/Polygon_mesh_processing/orient_polygon_soup.h>
#include <CGAL/Polygon_mesh_processing/polygon_soup_to_polygon_mesh.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_ratio_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_cost.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_filter.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_placement.h>
#include <CGAL/Surface_mesh_simplification/Edge_collapse_visitor_base.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/GarlandHeckbert_policies.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Polyhedral_envelope_filter.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_length_cost.h>
//bbox
#include <CGAL/Polygon_mesh_processing/bbox.h>

#include <CGAL/structure_point_set.h>

#include <CGAL/AABB_tree.h>
#include <CGAL/AABB_traits.h>
#include <CGAL/AABB_face_graph_triangle_primitive.h>

#include <CGAL/Polyhedron_3.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/vcm_estimate_edges.h>

#include <CGAL/property_map.h>

#include <CGAL/Timer.h>
#include <CGAL/Random.h>
#include <CGAL/Real_timer.h>
#include <CGAL/number_utils.h>

#include <CGAL/HalfedgeDS_vector.h>

#include <CGAL/memory.h>
#include <CGAL/Iterator_range.h>

#include <CGAL/Aff_transformation_3.h>

#include <CGAL/pointmatcher/register_point_sets.h>
#include <CGAL/OpenGR/compute_registration_transformation.h>

#include <CGAL/squared_distance_3.h>

#include <CGAL/Point_set_3.h>
#include <CGAL/Point_set_3/IO.h>

#include <CGAL/remove_outliers.h>
#include <CGAL/grid_simplify_point_set.h>
#include <CGAL/cluster_point_set.h>
#include <CGAL/compute_average_spacing.h>
#include <CGAL/jet_smooth_point_set.h>
#include <CGAL/jet_estimate_normals.h>
#include <CGAL/pca_estimate_normals.h>
#include <CGAL/mst_orient_normals.h>

#include <CGAL/Surface_mesh_approximation/approximate_triangle_mesh.h>

#include <CGAL/Polygon_mesh_processing/corefinement.h>

#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
#include <CGAL/Polygon_mesh_processing/border.h>
#include <CGAL/Polygon_mesh_processing/orientation.h>
#include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#include <CGAL/Polygon_mesh_processing/detect_features.h>

#include <CGAL/poisson_surface_reconstruction.h>
#include <CGAL/Advancing_front_surface_reconstruction.h>
#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/Scale_space_reconstruction_3/Jet_smoother.h>
#include <CGAL/Scale_space_reconstruction_3/Advancing_front_mesher.h>

#include <CGAL/Shape_detection/Efficient_RANSAC.h>

#include <CGAL/Shape_detection/Region_growing/Region_growing.h>
#include <CGAL/Shape_detection/Region_growing/Region_growing_on_polygon_mesh.h>

#include <CGAL/progressbar.h>

// =========================================================
// =========================================================
// TYPES

namespace VSA     = CGAL::Surface_mesh_approximation;
namespace SMS     = CGAL::Surface_mesh_simplification;
namespace PMP     = CGAL::Polygon_mesh_processing;
namespace params  = CGAL::parameters;
namespace fs      = std::__fs::filesystem;

using namespace std;

typedef CGAL::Simple_cartesian<double>                          cKernel;
typedef CGAL::Exact_predicates_inexact_constructions_kernel     Kernel;

typedef cKernel::FT                                             FT;

typedef Kernel::Point_3                                         Point_3;
typedef Kernel::Vector_3                                        Vector_3;
typedef Kernel::Segment_3                                       Segment_3;
typedef vector<size_t>                                          Polygon_3;

typedef CGAL::Polyhedron_3<Kernel>                              Polyhedron_3;
typedef Polyhedron_3::Vertex_iterator                           Vertex_iterator;

typedef CGAL::Surface_mesh<Point_3>                             Surface_mesh;
typedef Surface_mesh::Vertex_index                              vertex_descriptor;
typedef Surface_mesh::Face_index                                face_descriptor;

typedef CGAL::AABB_face_graph_triangle_primitive<Polyhedron_3>  Primitive;
typedef CGAL::AABB_traits<Kernel, Primitive>                    tTraits;
typedef CGAL::AABB_tree<tTraits>                                Tree;
typedef Tree::Point_and_primitive_id                            Point_and_primitive_id;

typedef pair<Point_3, Vector_3>                                 PointVectorPair;
typedef vector<PointVectorPair>                                 PointList;
typedef array<double,6>                                         Covariance;

typedef vector<PointVectorPair>                                 Pwn_vector;

typedef CGAL::Point_set_3<Point_3, Vector_3>                    Point_set_3;

typedef CGAL::First_of_pair_property_map<PointVectorPair>       Point_map;
typedef CGAL::Second_of_pair_property_map<PointVectorPair>      Normal_map;

typedef CGAL::Shape_detection::Efficient_RANSAC_traits
<Kernel, Pwn_vector, Point_map, Normal_map>                     Traits;
typedef CGAL::Shape_detection::Efficient_RANSAC<Traits>         Efficient_ransac;
typedef CGAL::Shape_detection::Plane<Traits>                    Plane;
typedef CGAL::Shape_detection::Sphere<Traits>                   Sphere;
typedef CGAL::Shape_detection::Torus<Traits>                    Torus;
typedef CGAL::Shape_detection::Cone<Traits>                     Cone;
typedef CGAL::Shape_detection::Cylinder<Traits>                 Cylinder;

typedef CGAL::Parallel_if_available_tag                         Concurrency_tag;

using Color = CGAL::IO::Color;

typedef boost::graph_traits<Surface_mesh>::vertex_descriptor        vertex_descriptor;
typedef boost::graph_traits<Surface_mesh>::halfedge_descriptor      halfedge_descriptor;
typedef boost::graph_traits<Surface_mesh>::face_descriptor          face_descriptor;
typedef boost::graph_traits<Surface_mesh>::edge_descriptor          edge_descriptor;

typedef SMS::Edge_profile<Surface_mesh>                                 Profile;

// Choose the type of a container for a polygon mesh.
#define USE_SURFACE_MESH

#if defined(USE_SURFACE_MESH)
    using Polygon_mesh    = CGAL::Surface_mesh<Point_3>;
    using Face_range      = typename Polygon_mesh::Face_range;
    using Neighbor_query  = CGAL::Shape_detection::Polygon_mesh::One_ring_neighbor_query<Polygon_mesh>;
    using Region_type     = CGAL::Shape_detection::Polygon_mesh::Least_squares_plane_fit_region<Kernel, Polygon_mesh>;
    using Sorting         = CGAL::Shape_detection::Polygon_mesh::Least_squares_plane_fit_sorting<Kernel, Polygon_mesh, Neighbor_query>;
#else
    using Polygon_mesh    = CGAL::Polyhedron_3<Kernel, CGAL::Polyhedron_items_3, CGAL::HalfedgeDS_vector>;
    using Face_range      = typename CGAL::Iterator_range<typename boost::graph_traits<Polygon_mesh>::face_iterator>;
    using Neighbor_query  = CGAL::Shape_detection::Polygon_mesh::One_ring_neighbor_query<Polygon_mesh, Face_range>;
    using Region_type     = CGAL::Shape_detection::Polygon_mesh::Least_squares_plane_fit_region<Kernel, Polygon_mesh, Face_range>;
    using Sorting         = CGAL::Shape_detection::Polygon_mesh::Least_squares_plane_fit_sorting<Kernel, Polygon_mesh, Neighbor_query, Face_range>;
#endif

using Region              = vector<size_t>;
using Regions             = vector<Region>;
using Vertex_to_point_map = typename Region_type::Vertex_to_point_map;
using Region_growing      = CGAL::Shape_detection::Region_growing<Face_range, Neighbor_query, Region_type, typename Sorting::Seed_map>;

// TYPES
// =========================================================
// =========================================================

// instance of function<bool(double)>
struct Progress_to_std_cerr_callback
{
  mutable size_t nb;
  CGAL::Real_timer timer;
  double t_start;
  mutable double t_latest;
  const string name;
  Progress_to_std_cerr_callback (const char* name)
    : name (name)
  {
    timer.start();
    t_start = timer.time();
    t_latest = t_start;
  }
  bool operator()(double advancement) const
  {
    // Avoid calling time() at every single iteration, which could
    // impact performances very badly
    ++ nb;
    if (advancement != 1 && nb % 100 != 0)
      return true;
    double t = timer.time();
    if (advancement == 1 || (t - t_latest) > 0.1) // Update every 1/10th of second
    {
      cerr << "\r" // Return at the beginning of same line and overwrite
                << name << ": " << int(advancement * 100) << "%";
      if (advancement == 1)
        cerr << endl;
      t_latest = t;
    }
    return true;
  }
};

struct Timeout_callback {
  mutable int nb;
  mutable CGAL::Timer timer;
  const double limit;
  Timeout_callback(double limit) :
  nb(0), limit(limit) {
    timer.start();
  }
  bool operator()(double advancement) const {
    // Avoid calling time() at every single iteration, which could
    // impact performances very badly.
    ++nb;
    if (nb % 1000 != 0)
      return true;
    // If the limit is reached, interrupt the algorithm.
    if (timer.time() > limit) {
      cerr << "Algorithm takes too long, exiting ("
                << 100.0 * advancement << "% done)" << endl;
      return false;
    }
    return true;
  }
};

// BGL property map which indicates whether an edge is marked as non-removable
struct Is_constrained_edge_map
{
  Surface_mesh* sm_ptr;
  typedef edge_descriptor                                       key_type;
  typedef bool                                                  value_type;
  typedef value_type                                            reference;
  typedef boost::readable_property_map_tag                      category;
  vector<vertex_descriptor> ind;

  Is_constrained_edge_map(Surface_mesh* sm, vector<vertex_descriptor>& ind) 
  {
    this->sm_ptr = sm;
    copy(ind.begin(), ind.end(), inserter(this->ind, this->ind.begin()));
  }
  // friend value_type get(const Surface_mesh& sm, vector<vertex_descriptor>& ind, const key_type& edge) 
  friend value_type get(const Is_constrained_edge_map& m, const key_type& edge) 
  {
    bool res = (find(m.ind.begin(), m.ind.end(), source(edge, *m.sm_ptr)) != m.ind.end()) || 
                (find(m.ind.begin(), m.ind.end(), target(edge, *m.sm_ptr)) != m.ind.end());
    return res;
  }
};
// Placement class
typedef SMS::Constrained_placement<SMS::Midpoint_placement<Surface_mesh>,
                                   Is_constrained_edge_map > Placement;

struct Stats
{
  size_t collected = 0;
  size_t processed = 0;
  size_t collapsed = 0;
  size_t non_collapsable = 0;
  size_t cost_uncomputable = 0;
  size_t placement_uncomputable = 0;
};

struct My_visitor : SMS::Edge_collapse_visitor_base<Surface_mesh>
{
  My_visitor(Stats* s) : stats(s) {}
  // Called during the collecting phase for each edge collected.
  void OnCollected(const Profile&, const boost::optional<double>&)
  {
    ++(stats->collected);
    cerr << "\rEdges collected: " << stats->collected << flush;
  }
  // Called during the processing phase for each edge selected.
  // If cost is absent the edge won't be collapsed.
  void OnSelected(const Profile&,
                  boost::optional<double> cost,
                  size_t initial,
                  size_t current)
  {
    ++(stats->processed);
    if(!cost)
      ++(stats->cost_uncomputable);
    if(current == initial)
      cerr << "\n" << flush;
    cerr << "\r" << current << flush;
  }
  // Called during the processing phase for each edge being collapsed.
  // If placement is absent the edge is left uncollapsed.
  void OnCollapsing(const Profile&,
                    boost::optional<Point> placement)
  {
    if(!placement)
      ++(stats->placement_uncomputable);
  }
  // Called for each edge which failed the so called link-condition,
  // that is, which cannot be collapsed because doing so would
  // turn the surface mesh into a non-manifold.
  void OnNonCollapsable(const Profile&)
  {
    ++(stats->non_collapsable);
  }
  // Called after each edge has been collapsed
  void OnCollapsed(const Profile&, vertex_descriptor)
  {
    ++(stats->collapsed);
  }
  Stats* stats;
};

struct Dummy_placement {
  template <typename Profile>
  boost::optional<typename Profile::Point> operator()(const Profile&) const
  {
    return boost::none;
  }
 template <typename Profile>
 boost::optional<typename Profile::Point> operator()(const Profile&, const boost::optional<typename Profile::Point>& op) const
  {
    return op;
  }
};

typedef SMS::GarlandHeckbert_plane_policies<Surface_mesh, Kernel>                  Classic_plane;
typedef SMS::GarlandHeckbert_probabilistic_plane_policies<Surface_mesh, Kernel>    Prob_plane;
typedef SMS::GarlandHeckbert_triangle_policies<Surface_mesh, Kernel>               Classic_tri;
typedef SMS::GarlandHeckbert_probabilistic_triangle_policies<Surface_mesh, Kernel> Prob_tri;


#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

// =========================================================
// =========================================================
// FUNCTIONS

// CONVERTATIONS && WRITING && READING

int STL2OFF(const string& fname_stl, const string& fname_off, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  Surface_mesh surface_mesh;
  vector<Point_3> points_ref;
  vector<Polygon_3> faces_ref;

  cout << "Reading from " << fname_stl << " ..." << endl;

  ifstream stl(fname_stl);
  if(!stl || !(CGAL::IO::read_STL(stl, points_ref, faces_ref)))
  {
    cerr << "Failed to read input mesh: " << fname_stl << endl;
    return EXIT_FAILURE;
  }

  cout << "Read " << points_ref.size() << " points" << endl;
  cout << "Points processing ..." << endl;

  PMP::orient_polygon_soup(points_ref, faces_ref);
  PMP::polygon_soup_to_polygon_mesh(points_ref, faces_ref, surface_mesh);

  if(!CGAL::is_triangle_mesh(surface_mesh))
  {
    cerr << "Input geometry is not triangulated." << endl;
    return EXIT_FAILURE;
  }

  cout << "Writing in " << fname_off << " ..." << endl;
  
  ofstream off(fname_off, ios_base::binary);
  CGAL::IO::write_OFF(off, points_ref, faces_ref);

  int n = (int)(points_ref.size());

  points_ref.clear();
  faces_ref.clear();
  surface_mesh.clear();

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return n;
}

int OBJ2OFF(const string& fname_obj, const string& fname_off, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  Surface_mesh surface_mesh;
  vector<Point_3> points_ref;
  vector<Polygon_3> faces_ref;

  cout << "Reading from " << fname_obj << " ..." << endl;

  ifstream obj(fname_obj);
  if(!obj || !(CGAL::IO::read_OBJ(obj, points_ref, faces_ref)))
  {
    cerr << "Failed to read input mesh: " << fname_obj << endl;
    return EXIT_FAILURE;
  }

  cout << "\nRead " << points_ref.size() << " points" << endl;
  cout << "Points processing ..." << endl;

  PMP::orient_polygon_soup(points_ref, faces_ref);
  PMP::polygon_soup_to_polygon_mesh(points_ref, faces_ref, surface_mesh);

  if(!CGAL::is_triangle_mesh(surface_mesh))
  {
    cerr << "Input geometry is not triangulated." << endl;
    return EXIT_FAILURE;
  }

  cout << "Writing in " << fname_off << " ..." << endl;
  
  ofstream off(fname_off, ios_base::binary);
  CGAL::IO::write_OFF(off, points_ref, faces_ref);

  int n = (int)(points_ref.size());

  points_ref.clear();
  faces_ref.clear();
  surface_mesh.clear();

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return n;
}

int OBJ2STL(const string& fname_obj, const string& fname_stl, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  Surface_mesh surface_mesh;
  vector<Point_3> points_ref;
  vector<Polygon_3> faces_ref;

  cout << "Reading from " << fname_obj << " ..." << endl;

  ifstream obj(fname_obj);
  if(!obj || !(CGAL::IO::read_OBJ(obj, points_ref, faces_ref)))
  {
    cerr << "Failed to read input mesh: " << fname_obj << endl;
    return EXIT_FAILURE;
  }

  cout << "\nRead " << points_ref.size() << " points" << endl;
  cout << "Points processing ..." << endl;

  PMP::orient_polygon_soup(points_ref, faces_ref);
  PMP::polygon_soup_to_polygon_mesh(points_ref, faces_ref, surface_mesh);

  if(!CGAL::is_triangle_mesh(surface_mesh))
  {
    cerr << "Input geometry is not triangulated." << endl;
    return EXIT_FAILURE;
  }

  cout << "Writing in " << fname_stl << " ..." << endl;
  
  ofstream stl(fname_stl, ios_base::binary);
  CGAL::IO::write_STL(stl, points_ref, faces_ref);

  int n = (int)(points_ref.size());

  points_ref.clear();
  faces_ref.clear();
  surface_mesh.clear();

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return n;
}

int ReadPtSetOff(const string& fname_off, Point_set_3& points, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  cout << "Reading from " << fname_off << " ..." << endl;

  ifstream off_ori (fname_off);
  if (!off_ori)
  {
    cerr << "Error: cannot read file " << fname_off << endl;
    return EXIT_FAILURE;
  }
  off_ori >> points;
  cout << "Read " << points.size () << " point(s)" << endl;
  if (points.empty())
    return EXIT_FAILURE;



  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

int OFF2XYZ (const string& fname_off, const string& fname_xyz, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  cout << "Reading from " << fname_off << " ..." << endl;

  PointList points;

  if(!CGAL::IO::read_points(CGAL::data_file_path(fname_off),
                            back_inserter(points),
                            CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())))
  {
    cerr << "Error: cannot read file " << fname_off << endl;
    return EXIT_FAILURE;
  }
  cout << "Read " << points.size () << " point(s)" << endl;
  if (points.empty())
    return EXIT_FAILURE;

  if(!CGAL::IO::write_XYZ(fname_xyz, points,
                          CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
                                           .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>())
                                           .stream_precision(17)))
    return EXIT_FAILURE;

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

int ReadPtVcPairOff(const string& fname_off, list<PointVectorPair>& points, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  cout << "\nReading from " << fname_off << " ..." << endl;

  if(!CGAL::IO::read_points(CGAL::data_file_path(fname_off),
                            back_inserter(points),
                            CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())))
  {
    cerr << "Error: cannot read file " << fname_off << endl;
    return EXIT_FAILURE;
  }
  cout << "Read " << points.size () << " point(s)" << endl;
  if (points.empty())
    return EXIT_FAILURE;

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

int ReadPtVcPairPLY(const string& fname_ply, list<PointVectorPair>& points, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  cout << "Reading from " << fname_ply << " ..." << endl;

  if(!CGAL::IO::read_points(CGAL::data_file_path(fname_ply),
                            back_inserter(points),
                            CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())))
  {
    cerr << "Error: cannot read file " << fname_ply << endl;
    return EXIT_FAILURE;
  }
  cout << "Read " << points.size () << " point(s)" << endl;
  if (points.empty())
    return EXIT_FAILURE;

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

int WritePtSetPwn(const string& fname_pwn, Point_set_3& points, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  cout << "Writing in " << fname_pwn << " ..." << endl;

  ofstream file(fname_pwn);
  for (int i = 0; i < points.number_of_points(); i++)
    cout << points.point(i) << " " << points.normal(i) << endl;
  file.close();

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

int WritePtVcPairPwn(const string& fname_pwn, list<PointVectorPair>& points, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  cout << "Writing in " << fname_pwn << " ..." << endl;

  ofstream file(fname_pwn);
  auto p = points.begin();
  for (int i = 0; i < (int)(points.size())-1; i++)
  {
    advance(p, 1);
    file << p->first << " " << p->second << endl;
  }
  file.close();

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

// CONVERTATIONS && WRITING && READING

int DecimateSurfaceMeshSTL(const string& fname_stl, const string& fname_out, double& stop_ratio, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  Surface_mesh surface_mesh;
  vector<Point_3> points_ref;
  vector<Polygon_3> faces_ref;

  ifstream stl(fname_stl);
  if(!stl || !(CGAL::IO::read_STL(stl, points_ref, faces_ref)))
  {
    cerr << "Failed to read input mesh: " << fname_stl << endl;
    return EXIT_FAILURE;
  }

  PMP::orient_polygon_soup(points_ref, faces_ref);
  PMP::polygon_soup_to_polygon_mesh(points_ref, faces_ref, surface_mesh);

  if(!CGAL::is_triangle_mesh(surface_mesh))
  {
    cerr << "Input geometry is not triangulated." << endl;
    return EXIT_FAILURE;
  }

  SMS::Count_ratio_stop_predicate<Surface_mesh> stop(stop_ratio);

  int r = SMS::edge_collapse(surface_mesh, stop);

  CGAL::IO::write_polygon_mesh(fname_out, surface_mesh, CGAL::parameters::stream_precision(17));

  points_ref.clear();
  faces_ref.clear();
  surface_mesh.clear();

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

int DecimateSurfaceMeshOFF(const string& fname_off, const string& fname_out, double& stop_ratio, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  Surface_mesh surface_mesh;

  ifstream off(fname_off);
  if(!off || !(CGAL::IO::read_OFF(off, surface_mesh)))
  {
    cerr << "Failed to read input mesh: " << fname_off << endl;
    return EXIT_FAILURE;
  }

  SMS::Count_ratio_stop_predicate<Surface_mesh> stop(stop_ratio);

  int r = SMS::edge_collapse(surface_mesh, stop);

  CGAL::IO::write_polygon_mesh(fname_out, surface_mesh, CGAL::parameters::stream_precision(17));

  surface_mesh.clear();

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

int PtSetRemoveOutliers(Point_set_3& points, int outlier_neighbors, double outlier_percentage, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  cout << "Removing outliers ..." << endl;

  typename Point_set_3::iterator rout_it = CGAL::remove_outliers<CGAL::Sequential_tag>
    (points,
     outlier_neighbors, // Number of neighbors considered for evaluation
     points.parameters().threshold_percent (outlier_percentage)); // Percentage of points to remove
  points.remove(rout_it, points.end());

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  cout << endl << points.number_of_removed_points()
            << " point(s) are removed as outliers, done in " << (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3) << "s" << endl;

  // Applying point set processing algorithm to a CGAL::Point_set_3
  // object does not erase the points from memory but place them in
  // the garbage of the object: memory can be freeed by the user.
  points.collect_garbage();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

int PtSetSimplification(Point_set_3& points, int simple_neighbors, double simple_ratio, double& time)
{
  chrono::steady_clock::time_point start_time = chrono::steady_clock::now();

  double spacing = CGAL::compute_average_spacing<CGAL::Sequential_tag> (points, simple_neighbors, CGAL::parameters::callback(Progress_to_std_cerr_callback("Computing average spacing")));

  cout << "\nGrid simplification ..." << endl;

  typename Point_set_3::iterator gsim_it = CGAL::grid_simplify_point_set (points, simple_ratio * spacing);
  points.remove(gsim_it, points.end());

  chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

  cout << points.number_of_removed_points()
            << " point(s) removed after simplification, done in " << (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3) << "s" << endl;

  points.collect_garbage();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  return EXIT_SUCCESS;
}

int EstimateNormals(list<PointVectorPair>& points, int flag, double& time)
{
  // Estimates normals direction.
  // Note: pca_estimate_normals() requiresa range of points
  // as well as property maps to access each point's position and normal.
  const int nb_neighbors = 18; // K-nearest neighbors = 3 rings
  if (flag == 0) // Use a fixed neighborhood radius
  {
    // First compute a spacing using the K parameter
    double spacing
        = CGAL::compute_average_spacing<Concurrency_tag>
          (points, nb_neighbors,
           CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>()));

    // Then, estimate normals with a fixed radius
    CGAL::pca_estimate_normals<Concurrency_tag>
        (points,
         0, // when using a neighborhood radius, K=0 means no limit on the number of neighbors returns
         CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
                          .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>())
                          .neighbor_radius(2. * spacing) // use 2*spacing as neighborhood radius
                          .callback(Progress_to_std_cerr_callback("PCA normal estimatition")));
  }
  else // Use a fixed number of neighbors
  {
    CGAL::pca_estimate_normals<Concurrency_tag>
        (points, nb_neighbors,
         CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
                          .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>())
                          .callback(Progress_to_std_cerr_callback("PCA normal estimatition")));
  }
  // Orients normals.
  // Note: mst_orient_normals() requires a range of points
  // as well as property maps to access each point's position and normal.
  list<PointVectorPair>::iterator unoriented_points_begin =
      CGAL::mst_orient_normals(points, nb_neighbors,
                               CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
                                                .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>())
                                                .callback(Progress_to_std_cerr_callback("MST normal estimatition")));
  // Optional: delete points with an unoriented normal
  // if you plan to call a reconstruction algorithm that expects oriented normals.
  points.erase(unoriented_points_begin, points.end());

  return EXIT_SUCCESS;
}

// FUNCTIONS
// =========================================================
// =========================================================

