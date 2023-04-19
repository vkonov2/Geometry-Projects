
#include "../MeshFunctions.h"

int main(int argc, char** argv)
{
	int flag = (argc > 1) ? atoi(argv[1]) : 0;
	int flag_size = (argc > 2) ? atoi(argv[2]) : 0;

	Surface_mesh surface_mesh;
	vector<Point_3> points_ref;
	vector<Polygon_3> faces_ref;

	string ori_fname_stl, ori_fname_off, ns_fname_stl, ns_fname_off, dc_fname_off, dc_fname_stl;
	string cmd, tmp, size;

	if (flag_size == 1)
		size = "m";
	else
		size = "";

	double stop_ratio, time;

	switch(flag)
	{
	case 0:
		{
			cout << ">>> enter argvs" << endl;
			break;
		}
	case 1:  // Cube
		{
			printf("\n      name\t\t time\t  pts\n=============================================\n");
			for (int i = 6; i < 10; i++)
			{
				ori_fname_stl = "../../../models/unit_1m" + size + "/originals/Cube/Cube" + to_string(i) + ".stl";
				ori_fname_off = "Cube" + to_string(i) + ".off";
				int n = STL2OFF(ori_fname_stl, ori_fname_off);

				for (int k = 1000; k < 10000; k+=2000)
				{ 
					dc_fname_off = "Cube" + to_string(i) + "Num" + to_string(k) + ".off";
					dc_fname_stl = "../../../models/unit_1m" + size + "/cgal/simplify/Cube/Cube" + to_string(i) + "Num" + to_string(k) + ".stl";
					tmp = "Cube" + to_string(i) + "Num" + to_string(k) + ".stl";
					stop_ratio = (double)(k) / ((double)(n));
					DecimateSurfaceMesh(ori_fname_stl, stop_ratio, time, dc_fname_off, dc_fname_stl);

					cout << tmp << "\t" << time << "s\t  " << k << "\n";
				}

				cout << "\n>>> size " << i << " done\n\n"; 
				cmd = "rm -f " + ori_fname_off;
				system(cmd.c_str());
			}
			break;
		}
	case 2:  // CubeSmoothed
		{
			printf("\n      name\t\t time\t  pts\n=============================================\n");
			for (int i = 6; i < 10; i++)
			{
				ori_fname_stl = "../../../models/unit_1m" + size + "/originals/CubeSmoothed/Cube" + to_string(i) + ".stl";
				ori_fname_off = "Cube" + to_string(i) + ".off";
				int n = STL2OFF(ori_fname_stl, ori_fname_off);

				for (int k = 1000; k < 10000; k+=2000)
				{ 
					dc_fname_off = "Cube" + to_string(i) + "Num" + to_string(k) + ".off";
					dc_fname_stl = "../../../models/unit_1m" + size + "/cgal/simplify/CubeSmoothed/Cube" + to_string(i) + "Num" + to_string(k) + ".stl";
					tmp = "Cube" + to_string(i) + "Num" + to_string(k) + ".stl";
					stop_ratio = (double)(k) / ((double)(n));
					DecimateSurfaceMesh(ori_fname_stl, stop_ratio, time, dc_fname_off, dc_fname_stl);

					cout << tmp << "\t" << time << "s\t  " << k << "\n";
				}

				cout << "\n>>> size " << i << " done\n\n"; 
				cmd = "rm -f " + ori_fname_off;
				system(cmd.c_str());
			}
			break;
		}
	case 3:  // CubeNoisy
		{
			printf("\n      name\t\t time\t  pts\n=============================================\n");
			for (int i = 6; i < 10; i++)
			{
				for (int j = 2; j < 10; j++)
				{
				  ns_fname_stl = "../../../models/unit_1m" + size + "/originals/CubeNoisy/" + to_string(j) + "/Cube" + to_string(i) + ".stl";
				  ns_fname_off = "Cube" + to_string(i) + "Ns" + to_string(j) + ".off";
				  int n = STL2OFF(ns_fname_stl, ns_fname_off);

				  for (int k = 1000; k < 10000; k+=2000)
				  { 
				    dc_fname_off = "Cube" + to_string(i) + "Ns" + to_string(j) + "Num" + to_string(k) + ".off";
				    dc_fname_stl = "../../../models/unit_1m" + size + "/cgal/simplify/CubeNoisy/" + to_string(j) + "/Cube" + to_string(i) + "Num" + to_string(k) + ".stl";
				    tmp = "Cube" + to_string(i) + "Ns" + to_string(j) + "Num" + to_string(k) + ".stl";
				    stop_ratio = (double)(k) / ((double)(n));
				    DecimateSurfaceMesh(ns_fname_stl, stop_ratio, time, dc_fname_off, dc_fname_stl);

				    cout << tmp << "\t" << time << "s\t  " << k << "\n";

				    cmd = "rm -f " + dc_fname_off;
				    system(cmd.c_str());  
				  }
				  cout << "\n>>> noise " << j << " done\n\n"; 

				  cmd = "rm -f " + ns_fname_off;
				  system(cmd.c_str());   
				}
				cout << "\n>>> size " << i << " done\n\n"; 
			}
			break;
		}
	case 4:  // CubeSmoothedNoisy
		{
			printf("\n      name\t\t time\t  pts\n=============================================\n");
			for (int i = 6; i < 10; i++)
			{
				for (int j = 2; j < 10; j++)
				{
				  ns_fname_stl = "../../../models/unit_1m" + size + "/originals/CubeSmoothedNoisy/" + to_string(j) + "/Cube" + to_string(i) + ".stl";
				  ns_fname_off = "Cube" + to_string(i) + "Ns" + to_string(j) + ".off";
				  int n = STL2OFF(ns_fname_stl, ns_fname_off);

				  for (int k = 1000; k < 10000; k+=2000)
				  { 
				    dc_fname_off = "Cube" + to_string(i) + "Ns" + to_string(j) + "Num" + to_string(k) + ".off";
				    dc_fname_stl = "../../../models/unit_1m" + size + "/cgal/simplify/CubeSmoothedNoisy/" + to_string(j) + "/Cube" + to_string(i) + "Num" + to_string(k) + ".stl";
				    tmp = "Cube" + to_string(i) + "Ns" + to_string(j) + "Num" + to_string(k) + ".stl";
				    stop_ratio = (double)(k) / ((double)(n));
				    DecimateSurfaceMesh(ns_fname_stl, stop_ratio, time, dc_fname_off, dc_fname_stl);

				    cout << tmp << "\t" << time << "s\t  " << k << "\n";

				    cmd = "rm -f " + dc_fname_off;
				    system(cmd.c_str());  
				  }
				  cout << "\n>>> noise " << j << " done\n\n"; 

				  cmd = "rm -f " + ns_fname_off;
				  system(cmd.c_str());   
				}
				cout << "\n>>> size " << i << " done\n\n"; 
			}
			break;
		}
	}

  return EXIT_SUCCESS;
}


















