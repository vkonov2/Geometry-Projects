
#include "../MeshFunctions.h"

int main (int argc, char** argv)
{
  string fname_off, fname_edges, fname_nonedges;
  chrono::steady_clock::time_point start_time, end_time;

  fname_off = "Poly.off";

  double time = 0.;

  double  R                   = (argc > 5) ? (double)(strtod(argv[5], NULL))  : 0.2;
  double  r                   = (argc > 6) ? (double)(strtod(argv[6], NULL))  : 0.1;

  double  threshold           = (argc > 7) ? (double)(strtod(argv[7], NULL))  : 0.16;

  list<PointVectorPair> points;
  ReadPtVcPairOff(fname_off, points, time);

  start_time = chrono::steady_clock::now();
  vector<Covariance> cov;
  CGAL::First_of_pair_property_map<PointVectorPair> point_map;

  cout << "Computing Voronoi Covariance Measure ..." << endl;
  CGAL::compute_vcm(points, cov, R, r, CGAL::parameters::point_map(point_map).geom_traits (Kernel()));
  end_time = chrono::steady_clock::now();
  cout << "\nComputed VCM for " << points.size() << " points, done in " << (double)(chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()/1e+3) << "s" << endl;

  time += (double)(chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()/1e+3);

  dir = to_string(outlier_neighbors) + "_" +  to_string(outlier_percentage) + "_" + to_string(simple_neighbors) + "_" + to_string(simple_ratio) + "_" + to_string(R) + "_" + to_string(r);
  cmd = "mkdir data/" + dir;
  system(cmd.c_str());

  threshold = 0.005;
  fname_edges = "PtsEdges" + to_string(threshold) + ".xyz";
  fname_nonedges = "PtsNonEdges" + to_string(threshold) + ".xyz";

  start_time = chrono::steady_clock::now();

  ofstream output_edg(fname_edges);
  ofstream output_nonedg(fname_nonedges);

  int cnt = 0;
  for(const PointVectorPair& p : points)
  {
    if(CGAL::vcm_is_on_feature_edge(cov[cnt], threshold))
      output_edg << p.first << endl;
    else
      output_nonedg << p.first << endl;
    cnt++;
  }

  output_edg.close();
  output_nonedg.close();

  end_time = chrono::steady_clock::now();
  time += (double)(chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count()/1e+3);

  cov.clear();
  points.clear();

  cout << "time: " << time << "s" << endl;

  return 0;
}