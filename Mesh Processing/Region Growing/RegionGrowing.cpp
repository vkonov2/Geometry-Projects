
#include "../MeshFunctions.h"

int main(int argc, char** argv) 
{
  string cmd, dir, fname_off;

  chrono::steady_clock::time_point start_time, end_time;

  fname_off = "../../../models/PolyOriginal.off";

  ifstream in(fname_off);
  if (!in) 
  {
    cout << "Error: cannot read the file --> " << fname_off << endl;
    return EXIT_FAILURE;
  }

  Polygon_mesh polygon_mesh;
  in >> polygon_mesh;

  in.close();

  const Face_range face_range = faces(polygon_mesh);
  cout << "* polygon mesh with " << face_range.size() << " faces is loaded" << endl;

  start_time = chrono::steady_clock::now();

  const FT     max_distance_to_plane = (argc > 1) ? FT((double)(strtod(argv[1], NULL))) : FT(5e-3);
  const FT     max_accepted_angle    = (argc > 2) ? FT((double)(strtod(argv[2], NULL))) : FT(10);
  const size_t min_region_size       = (argc > 3) ? atoi(argv[3]) : 100;

  Neighbor_query neighbor_query(polygon_mesh);

  const Vertex_to_point_map vertex_to_point_map(get(CGAL::vertex_point, polygon_mesh));
  Region_type region_type(
    polygon_mesh,
    max_distance_to_plane, max_accepted_angle, min_region_size,
    vertex_to_point_map);

  Sorting sorting(
    polygon_mesh, neighbor_query,
    vertex_to_point_map);
  sorting.sort();

  Region_growing region_growing(
    face_range, neighbor_query, region_type,
    sorting.seed_map());

  Regions regions;
  region_growing.detect(back_inserter(regions));

  cout << "* " << regions.size() << " regions have been found" << endl;

  end_time = chrono::steady_clock::now();

  ofstream out;

  int i = 0;
  for (const auto& region : regions) 
  {
    string flnm = "planes/indices/plane" + to_string(i) + ".txt";
    out.open(flnm, ios_base::app);
    for (const auto index : region)
    {
      // out << (Polygon_mesh::Face_index(static_cast<Polygon_mesh::size_type>(index))) << "\t";
      out << index << "\t";
    }
    out.close();
    i++;
  }

  // cout << "* polygon mesh is saved in " << fullpath << endl;

  cout << endl << "region_growing_on_polygon_mesh finished in " << (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3) << " secs" << endl << endl;

  return EXIT_SUCCESS;
}