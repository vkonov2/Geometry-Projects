
#include "../MeshFunctions.h"

int main (int argc, char** argv)
{
  int threshold = (argc > 2) ? atoi(argv[2]) : 300;

  Point_set_3 points;

  ifstream ifile("PtsNonEdges.xyz");
  
  ifile >> points;
  ifile.close();

  // Add a cluster map
  Point_set_3::Property_map<int> cluster_map = points.add_property_map<int>("cluster", -1).first;

  // Compute average spacing
  double spacing = CGAL::compute_average_spacing<CGAL::Parallel_if_available_tag> (points, 12);
  cerr << "Spacing = " << spacing << endl;

  // Adjacencies stored in vector
  vector<pair<size_t, size_t> > adjacencies;

  int i = 50;
  // Compute clusters
  CGAL::Real_timer t;
  t.start();
  size_t nb_clusters
    = CGAL::cluster_point_set(points, cluster_map,
                              points.parameters().neighbor_radius((0.5+(double)(i)/100.0)*spacing)
                                                 .adjacencies(back_inserter(adjacencies)));
  t.stop();
  cerr << i << " >>> Found " << nb_clusters << " clusters with " << adjacencies.size()
            << " adjacencies in " << t.time() << " seconds" << endl;

  // Output a colored PLY file
  Point_set_3::Property_map<unsigned char> red = points.add_property_map<unsigned char>("red", 0).first;
  Point_set_3::Property_map<unsigned char> green = points.add_property_map<unsigned char>("green", 0).first;
  Point_set_3::Property_map<unsigned char> blue = points.add_property_map<unsigned char>("blue", 0).first;

  vector<int> cl_sz((int)(nb_clusters));
  for (int j = 0; j < (int)(nb_clusters); j++)
    cl_sz[j] = 0;

  for(Point_set_3::Index idx : points)
  {
    cl_sz[(int)(cluster_map[idx])]++;

    // One color per cluster
    CGAL::Random rand (cluster_map[idx]);
    red[idx] = rand.get_int(64, 192);
    green[idx] = rand.get_int(64, 192);
    blue[idx] = rand.get_int(64, 192);
  }

  vector<pair<int, vector<int> > > cl;

  for (int j = 0; j < (int)(nb_clusters); j++)
  {
    if (cl_sz[j] > threshold)
    {
      pair<int, vector<int> > local;

      local.first = j;
      for(Point_set_3::Index idx : points)
      {
        if ((int)(cluster_map[idx]) == j)
          local.second.push_back(idx);
      }

      cl.push_back(local);
      local.second.clear();
    }
  }

  // for (int j = 0; j < (int)(cl.size()); j++)
  //   cout << cl[j].first  << " --> " << cl[j].second.size() << endl;

  string fname;
  for (int j = 0; j < (int)(cl.size()); j++)
  {
    fname = "clusters/cluster" + to_string(j) + "_" + to_string(cl[j].second.size()) + ".ply";

    ofstream out(fname, fstream::app);

    out << "ply" << endl;
    out << "format ascii 1.0" << endl;
    out << "element vertex " << (int)(cl[j].second.size()) << endl;
    out << "property double x" << endl;
    out << "property double y" << endl;
    out << "property double z" << endl;
    out << "end_header" << endl;

    for (int k = 0; k < (int)(cl[j].second.size()); k++)
      out << points.point(cl[j].second[k]) << endl;

    out.close();
  }

  ofstream ofile("out" + to_string(0.5+(double)(i)/100.0) + ".ply");
  // CGAL::IO::set_binary_mode(ofile);
  ofile << points;
  ofile.close();

  adjacencies.clear();
  cl_sz.clear();
  cl.clear();
  points.clear();

  return EXIT_SUCCESS;
}