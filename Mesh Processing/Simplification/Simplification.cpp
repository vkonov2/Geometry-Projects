
#include "../MeshFunctions.h"

int main(int argc, char** argv) 
{
  int method = (argc > 1) ? atoi(argv[1]) : 1;
  int policy = (argc > 2) ? atoi(argv[2]) : 0;
  int orig   = (argc > 3) ? atoi(argv[3]) : 0;
  int simp   = (argc > 4) ? atoi(argv[4]) : 1;

  string fname_off, fname_off_sp, fname_edges_ind, fname_edges_ind_sp;
  double time = 0.;
  size_t nb_border_edges;
  int i, cnt, num;
  chrono::steady_clock::time_point start_time, end_time;

  fname_off          = "PolySimplified/Poly" + to_string(orig) + ".off";
  fname_off_sp       = "PolySimplified/Poly" + to_string(simp) + ".off";
  fname_edges_ind    = "../vertices_from_faces/planes/plane" + to_string(orig) + ".txt";

  ifstream is(fname_off);

  Surface_mesh surface_mesh, surface_mesh_ori;
  Surface_mesh::Property_map<halfedge_descriptor, pair<Point_3, Point_3> > constrained_halfedges;
  constrained_halfedges = surface_mesh.add_property_map<halfedge_descriptor, pair<Point_3, Point_3> >("h:vertices").first;

  if(!is || !(is >> surface_mesh))
  {
    cerr << "Failed to read input mesh: " << fname_off << endl;
    return EXIT_FAILURE;
  }

  if(!CGAL::is_triangle_mesh(surface_mesh))
  {
    cerr << "Input geometry is not triangulated." << endl;
    return EXIT_FAILURE;
  }

  surface_mesh_ori = surface_mesh;

  start_time = chrono::steady_clock::now();

  Stats stats;
  My_visitor vis(&stats);

  vector<int> ind_id;
  unsigned long int vd_id;

  vector<Point_3> verts;
  vector<vertex_descriptor> ind;
  vertex_descriptor vd;

  ifstream edg_ind(fname_edges_ind);

  while(!edg_ind.eof())
  {
    edg_ind >> vd_id;
    // cout << vd_id << endl;
    ind_id.push_back(vd_id);
  }

  edg_ind.close();

  num = surface_mesh.number_of_vertices();
  cout << "Getting constrained indices ..." << endl;
  for(vertex_descriptor vd : vertices(surface_mesh))
  {
    // if (find(ind_id.begin(), ind_id.end(), vd.idx()) != ind_id.end())
    // {
    //   ind.push_back(vd);
    //   verts.push_back(surface_mesh.point(vd));
    // }

    if (find(ind_id.begin(), ind_id.end(), vd.idx()) != ind_id.end())
      continue;
    else
    {
      ind.push_back(vd);
      verts.push_back(surface_mesh.point(vd));
    }

    printProgress((double)(vd.idx())/(double)(num));
  }

  ind_id.clear();
  cout << endl;
  
  end_time = chrono::steady_clock::now();
  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  // Contract the surface mesh as much as possible
  SMS::Count_stop_predicate<Surface_mesh> stop(0);
  // SMS::Count_ratio_stop_predicate<Surface_mesh> stop(ratio);
  Is_constrained_edge_map bem(&surface_mesh, ind);

  start_time = chrono::steady_clock::now();

  if (method == 0)
  {
    // This the actual call to the simplification algorithm.
    // The surface mesh and stop conditions are mandatory arguments.
    cout << "\nCollapsing as many edges of mesh: " << fname_off << " as possible with Lindstrom-Turk policy ..." << endl;
    cnt = SMS::edge_collapse(surface_mesh, stop,
                               CGAL::parameters::edge_is_constrained_map(bem)
                                                .get_placement(Placement(bem))
                                                .visitor(vis));
  }
  else
  {
    if (policy == 1)
    {
      typedef Classic_tri::Get_cost                                        Cost;
      typedef Classic_tri::Get_placement                                   Placement;
      typedef SMS::Bounded_normal_change_placement<Placement>              Bounded_placement;

      SMS::Bounded_normal_change_filter<> filter;

      Classic_tri policies(surface_mesh);
      // const Cost& cost = policies.get_cost();
      auto cost = CGAL::Surface_mesh_simplification::Edge_length_cost<Surface_mesh>();
      const Placement& placement = policies.get_placement();
      Bounded_placement res_placement(placement);

      // This the actual call to the simplification algorithm.
      // The surface mesh and stop conditions are mandatory arguments.
      cout << "\nCollapsing as many edges of mesh: " << fname_off << " as possible with Garland-Heckbert policy: classic triangle ..." << endl;
      cnt = SMS::edge_collapse(surface_mesh, stop,
                                 CGAL::parameters::get_cost(CGAL::Surface_mesh_simplification::Edge_length_cost<Surface_mesh>())
                                                  // .edge_is_constrained_map(bem)
                                                  .get_placement(res_placement)
                                                  .filter(filter)
                                                  .visitor(vis));
    }
    else if (policy == 2)
    {
      typedef Prob_plane::Get_cost                                        Cost;
      typedef Prob_plane::Get_placement                                   Placement;
      typedef SMS::Bounded_normal_change_placement<Placement>             Bounded_placement;

      Prob_plane policies(surface_mesh);
      const Cost& cost = policies.get_cost();
      const Placement& placement = policies.get_placement();
      Bounded_placement res_placement(placement);

      // This the actual call to the simplification algorithm.
      // The surface mesh and stop conditions are mandatory arguments.
      cout << "\nCollapsing as many edges of mesh: " << fname_off << " as possible with Garland-Heckbert policy: probabalistic plane ..." << endl;
      cnt = SMS::edge_collapse(surface_mesh, stop,
                                 CGAL::parameters::get_cost(cost)
                                                  .edge_is_constrained_map(bem)
                                                  .get_placement(res_placement)
                                                  .visitor(vis));
    }
    else if (policy == 3)
    {
      typedef Prob_tri::Get_cost                                        Cost;
      typedef Prob_tri::Get_placement                                   Placement;
      typedef SMS::Bounded_normal_change_placement<Placement>                Bounded_placement;

      Prob_tri policies(surface_mesh);
      const Cost& cost = policies.get_cost();
      const Placement& placement = policies.get_placement();
      Bounded_placement res_placement(placement);

      // This the actual call to the simplification algorithm.
      // The surface mesh and stop conditions are mandatory arguments.
      cout << "\nCollapsing as many edges of mesh: " << fname_off << " as possible with Garland-Heckbert policy: probabalistic triangle ..." << endl;
      cnt = SMS::edge_collapse(surface_mesh, stop,
                                 CGAL::parameters::get_cost(cost)
                                                  .edge_is_constrained_map(bem)
                                                  .get_placement(res_placement)
                                                  .visitor(vis));
    }
    else
    {
      typedef Classic_plane::Get_cost                                        Cost;
      typedef Classic_plane::Get_placement                                   Placement;
      typedef SMS::Bounded_normal_change_placement<Placement>                Bounded_placement;

      Classic_plane policies(surface_mesh);
      const Cost& cost = policies.get_cost();
      const Placement& placement = policies.get_placement();
      Bounded_placement res_placement(placement);

      // This the actual call to the simplification algorithm.
      // The surface mesh and stop conditions are mandatory arguments.
      cout << "\nCollapsing as many edges of mesh: " << fname_off << " as possible with Garland-Heckbert policy: classic plane ..." << endl;
      cnt = SMS::edge_collapse(surface_mesh, stop,
                                 CGAL::parameters::get_cost(cost)
                                                  .edge_is_constrained_map(bem)
                                                  .get_placement(res_placement)
                                                  .visitor(vis));
    }
  }

  

  cout      << "\n\nEdges collected: "  << stats.collected
            << "\nEdges proccessed: " << stats.processed
            << "\nEdges collapsed: "  << stats.collapsed
            << endl
            << "\nEdges not collapsed due to topological constraints: "  << stats.non_collapsable
            << "\nEdge not collapsed due to cost computation constraints: "  << stats.cost_uncomputable
            << "\nEdge not collapsed due to placement computation constraints: " << stats.placement_uncomputable
            << endl;

  end_time = chrono::steady_clock::now();

  time += (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3);

  cout << "\nFinished!\n" << cnt << " edges removed.\n"
            << surface_mesh.number_of_edges() << " final edges in " << (double)(chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count()/1e+3) << "s" << endl;

  cout << "Writing simplificated mesh in " << fname_off_sp << " ..." << endl;
  CGAL::IO::write_polygon_mesh(fname_off_sp, surface_mesh, CGAL::parameters::stream_precision(17));

  // cout << "Saving updated constrained indices ..." << endl;

  // ofstream edg_ind_sp(fname_edges_ind_sp);

  // vector<int> ind_sp;

  // start_time = chrono::steady_clock::now();
  // num = (int)(surface_mesh.number_of_halfedges());
  // for(halfedge_descriptor hd : halfedges(surface_mesh))
  // {
  //   if (find(verts.begin(), verts.end(), surface_mesh.point(source(hd, surface_mesh))) != verts.end())
  //     ind_sp.push_back((int)(source(hd, surface_mesh).idx()));
      
  //   if (find(verts.begin(), verts.end(), surface_mesh.point(target(hd, surface_mesh))) != verts.end())
  //     ind_sp.push_back((int)(target(hd, surface_mesh).idx()));

  //   // printProgress((double)(hd.idx())/(double)(num));
  // }

  // sort( ind_sp.begin(), ind_sp.end() );
  // ind_sp.erase( unique( ind_sp.begin(), ind_sp.end() ), ind_sp.end() );

  // for (auto id : ind_sp)
  //   edg_ind_sp << id << endl;

  // edg_ind_sp.close();

  surface_mesh.clear();
  surface_mesh_ori.clear();
  ind.clear();
  verts.clear();
  
  cout << "\n\nprocessing time: " << time << "s" << endl;

  return EXIT_SUCCESS;
}



