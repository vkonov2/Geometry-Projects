#include "LoadSave.h"
#include <vector>
#include <set>
#include <fstream>

using namespace std;

POLYHEDRON* read_OFF(const char* prefix, const char* fname)
{
    const string fullname_string = string(prefix) + string(fname);
    const char* fullname = fullname_string.c_str(); 
    int len, id, r;

    FILE* f = fopen(fullname, "r");

    if(!f)
    {
        printf("Can't open %s\n", fullname);
        return 0;
    }

    int nverts, nbones, nsides, nverts_per_side, c;

    fseek(f, 4, SEEK_SET);

    r = fscanf(f, "%d%d%d", &nverts, &nsides, &nbones);

    if(r != 3)
        printf("Error\n");

    POLYHEDRON* P = new POLYHEDRON(nverts, nbones, nsides);

    double x, y, z;

    for (int i = 0; i < nverts; i++)
    {
        r = fscanf(f, "%lf%lf%lf", &x, &y, &z);
    
        P->vertex[i].x = x;
        P->vertex[i].y = y;
        P->vertex[i].z = z;
    }

    for (int i = 0; i < nsides; i++)
    {
        r = fscanf(f, "%d", &nverts_per_side);
        
        int size = nverts_per_side + 1;
        PHINDEX* vlist = new PHINDEX[size];
        if (!vlist)
        {
            return 0;
        }
        for (int j = 0; j < nverts_per_side; j++)
        {
            r = fscanf(f, "%d", &c);
            
            vlist[j] = c;
        }
        vlist[nverts_per_side] = vlist[0];
        P->side[i].copy_vlist(vlist, size);

        len = length(P->side[i].vlist);
        VECTOR* pts = new VECTOR[len];

        for (int k = 0; k < len; k++)
        {
            id = P->side[i].vlist[k];

            pts[k] = P->vertex[id];
        }

        DefinePlaneCoeff(len, pts, P->side[i].plane);

        delete[] pts;
        delete[] vlist;
    }

    P->initbones();
    fclose(f);

    return P;
}

IO_RET_CODE write_OFF(const POLYHEDRON* P, const char* prefix, const char* fname)
{
    const string fullname_string = string(prefix) + string(fname);
    const char* fullname = fullname_string.c_str();
    const string log_string = string(prefix) + "\\logger_print.txt";
    int nverts_per_side;

    FILE* f = fopen(fullname, "w");
    if(!f)
    {
        printf("Can't open %s\n", fullname);
        return IO_WRITE_ERROR;
    }

    if (!fprintf(f, "%s\n%d %d %d\n", "OFF", P->nverts, P->nsides, P->nbones))
    {
        return IO_WRITE_ERROR;
    }

    for (int i = 0; i < P->nverts; i++)
        if (!fprintf(f, "%18.15lf %18.15lf %18.15lf\n", P->vertex[i].x, P->vertex[i].y, P->vertex[i].z))
        {
            return IO_WRITE_ERROR;
        }

    for (int i = 0; i < P->nsides; i++)
    {
        nverts_per_side = length(P->side[i].vlist) - 1;

        if (!fprintf(f, "%d ", nverts_per_side))
        {
            return IO_WRITE_ERROR;
        }

        for (int j = 0; j < nverts_per_side; j++)
        {
            if (!fprintf(f, "%d ", P->side[i].vlist[j]))
            {
                return IO_WRITE_ERROR;
            }
        }
        fprintf(f, "\n");
    }

    if (!fprintf(f, "# EDGES (BONES)  id  facets[2]  vertices[2]    (actual bones %d/2)\n", P->nbones))
    {
        return IO_WRITE_ERROR;
    }

    for (int i = 0; i < P->nbones; i++) {
        if (!fprintf(f, " %3d   %3d %3d   %3d %3d\n", i, P->bone[i].s0, P->bone[i].s1, P->bone[i].v0, P->bone[i].v1))
        {
            return IO_WRITE_ERROR;
        }
    }

    fclose(f);

    return IO_OK;
}

IO_RET_CODE PrintTextPolyhedron(const POLYHEDRON* pPoly, const char* prefix, const char* fname)
{
    int i, j, k, nb = 0;
    const string fullname_string = string(prefix) + string(fname);
    const char* fullname = fullname_string.c_str();
    const string log_string = string(prefix) + "\\logger_print.txt";
    
    FILE* f = fopen(fullname, "w");
    if(!f)
    {
        printf("Can't open %s\n", fullname);
        return IO_WRITE_ERROR;
    }

    if (!fprintf(f, "# POLYHEDRON  vertices  sides  bones\n  %d  %d  %d\n# VERTICES\n", pPoly->nverts, pPoly->nsides, pPoly->nbones))
    {
        return IO_WRITE_ERROR;
    }

    for (i = 0; i < pPoly->nverts; i++) {
        if (!fprintf(f, " %3d", i))
        {
            return IO_WRITE_ERROR;
        }
        if (!fprintf(f, "  %18.15f %18.15f %18.15f\n", pPoly->vertex[i].x, pPoly->vertex[i].y, pPoly->vertex[i].z))
        {
            return IO_WRITE_ERROR;
        }
    }
    if (!fprintf(f, "# FACETS (SIDES)  id  num_verts  verts_list  \\  plane coeff\n"))
    {
        return IO_WRITE_ERROR;
    }
    for (i = 0; i < pPoly->nsides; i++) {
        k = length(pPoly->side[i].vlist) - 1;
        nb += k;
       
        if (!fprintf(f, " %3d  %3d    ", i, k))
        {
            return IO_WRITE_ERROR;
        }
        for (j = 0; j < k; j++) {
            if (!fprintf(f, " %3d", pPoly->side[i].vlist[j]))
            {
                return IO_WRITE_ERROR;
            }
        }
        if (!fprintf(f, "\n      %18.15f %18.15f %18.15f %18.15f\n",
            pPoly->side[i].plane.norm.x, pPoly->side[i].plane.norm.y, pPoly->side[i].plane.norm.z, pPoly->side[i].plane.dist))
        {
            return IO_WRITE_ERROR;
        }
    }
    if (!fprintf(f, "# EDGES (BONES)  id  facets[2]  vertices[2]    (actual bones %d/2)\n", nb))
    {
        return IO_WRITE_ERROR;
    }
    for (i = 0; i < pPoly->nbones; i++) {
        if (!fprintf(f, " %3d   %3d %3d   %3d %3d\n", i, pPoly->bone[i].s0, pPoly->bone[i].s1, pPoly->bone[i].v0, pPoly->bone[i].v1))
        {
            return IO_WRITE_ERROR;
        }
    }

    fclose(f);

    return IO_OK;
}

POLYHEDRON* LoadTextPolyhedron(const char* prefix, const char* fname)
{

    const string fullname_string = string(prefix) + string(fname);
    const char* fullname = fullname_string.c_str();

    int i, j, k;

    // printf("here\n");

    FILE* f = fopen(fullname, "r");
    if(!f)
    {
        printf("Can't open %s\n", fullname);
        return 0;
    }
    char c = 'a';

    int nverts = 0, nsides = 0, nbones = 0;
    int ret, ivert, iside;

    while(fscanf(f, "%c", &c) && c != '\n')
    {
        continue;
    }

    ret = fscanf(f, "  %d %d %d", &nverts, &nsides, &nbones);
    if(ret != 3)
        printf("Bad file: %d%d%d\n", nverts, nsides, nbones);

    // printf("nverts, nsides, nbones: %d %d %d\n", nverts, nsides, nbones);

    POLYHEDRON* pPoly = new POLYHEDRON(nverts, nbones, nsides);

    ret = fscanf(f, "%c", &c);
    // printf("c = %c\n", c);
    ret = fscanf(f, "%c", &c);
    // printf("c = %c\n", c);
    while(fscanf(f, "%c", &c) && c != '\n')
    {
        // printf("c = %c\n", c);
        continue;
    }

    for (i = 0; i < nverts; i++) {
        ret = fscanf(f, "%d%lf%lf%lf", &ivert, &(pPoly->vertex[i].x), &(pPoly->vertex[i].y), &(pPoly->vertex[i].z));
        // printf("ivert, pPoly->vertex[i].x, pPoly->vertex[i].y,pPoly->vertex[i].z: %d%lf%lf%lf\n", ivert, pPoly->vertex[i].x, pPoly->vertex[i].y,pPoly->vertex[i].z);
        if(ret != 4)
            printf("Bad vertices in file: %d%lf%lf%lf\n", ivert, pPoly->vertex[i].x, pPoly->vertex[i].y,pPoly->vertex[i].z);
    }

    int nv;
    VECTOR3 norm;
    double dist;

    ret = fscanf(f, "%c", &c);
    // printf("c = %c\n", c);
    ret = fscanf(f, "%c", &c);
    // printf("c = %c\n", c);
    while(fscanf(f, "%c", &c) && c != '\n')
    {
        continue;
    }

    for (i = 0; i < nsides; i++) {
        ret = fscanf(f, "%d%d", &iside, &nv);
        PHINDEX* vlist = new PHINDEX[nv + 1];
        VECTOR pts[3];

        for (j = 0; j < nv; j++)
        {
            ret = fscanf(f, "%d", &k);
            vlist[j] = k;
            pts[j] = pPoly->vertex[k];
        }

        PLANE Plane;
        DefinePlaneCoeff(3, pts, Plane);

        vlist[nv] = vlist[0];
        // printf("Bfr\n");
        pPoly->side[i].copy_vlist(vlist, nv + 1);
        // printf("Aft\n");
        ret = fscanf(f, "%lf%lf%lf%lf", &(norm.x), &(norm.y), &(norm.z), &dist);
        pPoly->side[i].plane.norm = Plane.norm / Plane.norm.abs();
        pPoly->side[i].plane.dist = Plane.dist / Plane.norm.abs();



        delete[] vlist;
    }

    // ret = fscanf(f, "%c", &c);
    // while(fscanf(f, "%c", &c) && c != '\n')
    // {
    //     continue;
    // }

    // vector<int> set1;

    // vector<int> set2;  

    // int id_1, id_2;
    // int ival;
    
    // for (i = 0; i < nbones; i++) 
    // {
    //     fscanf(f, "%d%d%d%d%d", &ival, &id_1, &id_2, &ival, &ival);

    //     set1.push_back(id_1);
    //     set2.push_back(id_2);
    // }

    // fclose(f);

    // int count, count_cur = 0;

    // for (i = 0; i < nbones; i++)
    // {
    //     count = std::count(set1.begin(), set1.end(), i);

    //     PHINDEX* slist = new PHINDEX[count + 1];
    //     for (j = 0; j < count; j++)
    //         pPoly->side[i].slist.push_back(set2[count_cur+j]);

    //     count_cur += count;
    // }

    // for (i = 0; i < nsides; i++)
    // {
    //     for (j = 0; j < i; j++)
    //     {
    //         for (k = 0; k < ((int)(pPoly->side[j].slist.size())); k++)
    //         {
    //             if (pPoly->side[j].slist[k] == i)
    //                 pPoly->side[i].slist.push_back(j);
    //         }
    //     }

    //     set <int> unique;
    //     unique.clear();

    //     for (int id : pPoly->side[i].slist)
    //         unique.insert(id);

    //     count = unique.size();
    //     pPoly->side[i].slist.clear();
    //     for (int id : unique)
    //         pPoly->side[i].slist.push_back(id);

    //     pPoly->side[i].slist.push_back(pPoly->side[i].slist[0]);

    //     unique.clear();
    // }

    // cout << pPoly->side[i].slist << endl;
    // k = 7;
    // printf ("%d %d %d %d\n", pPoly->side[k].slist[0], pPoly->side[k].slist[1], pPoly->side[k].slist[2], pPoly->side[k].slist[3]);

    pPoly->initbones();

    // printf("Here\n");

    vector<int> set1;

    vector<int> set2;  

    int id_1, id_2;

    for (i = 0; i < nbones; i++) 
    {
        // fscanf(f, "%d%d%d%d%d", &ival, &id_1, &id_2, &ival, &ival);
        id_1 = pPoly->bone[i].s0;
        id_2 = pPoly->bone[i].s1;

        set1.push_back(id_1);
        set2.push_back(id_2);
    }

    fclose(f);

    // printf("Here\n");

    int count, count_cur = 0;

    for (i = 0; i < nbones; i++)
    {
        count = std::count(set1.begin(), set1.end(), i);

        for (j = 0; j < count; j++)
            pPoly->side[i].slist.push_back(set2[count_cur+j]);

        count_cur += count;
    }

    for (i = 0; i < nsides; i++)
    {
        for (j = 0; j < i; j++)
        {
            for (k = 0; k < ((int)(pPoly->side[j].slist.size())); k++)
            {
                if (pPoly->side[j].slist[k] == i)
                    pPoly->side[i].slist.push_back(j);
            }
        }

        set <int> unique;
        unique.clear();

        for (int id : pPoly->side[i].slist)
            unique.insert(id);

        count = unique.size();
        // printf("count = %d\n", count);
        pPoly->side[i].slist.clear();
        for (int id : unique)
        {
            // cout << id << endl;
            pPoly->side[i].slist.push_back(id);
        }

        pPoly->side[i].slist.push_back(pPoly->side[i].slist[0]);

        unique.clear();

        // printf("Exit\n");
    }

    set1.clear();
    set2.clear();

    // printf("Here\n");

    return pPoly;
}

// int LoadSTLPolyhedron(const char* fname)
// {
//     vector < PHSIDE > Faces
//     vector < VECTOR > Verts

//     ifstream f;
//     string line;
//     char str1[80], str2[80];
//     int count = 0;
//     double n1, n2, n3;

//     f.open(fname);
//     if (!f.is_open())
//     {
//         printf("Can't open %s\n", fname);
//         return -1;
//     }

//     getline(f, line);
//     if (line != "solid 0")
//     {
//         printf("Bad .obj format\n");
//         return -2;
//     }
//     line.clear();

//     Vector3 v[3], n;
//     //vector < Vector3 > Verts; // , Norms;
//     //vector < SimpleFace > SimpleFaces;

//     while (true)
//     {
//         getline(f, line);
//         if (line == "endsolid 0")
//             break;

//         //cout << line << endl;

//         sscanf((line).c_str(), "%s%s%lf%lf%lf", str1, str2, &n1, &n2, &n3);
//         str1[0] = '\0';
//         str2[0] = '\0';
//         //printf("%lf %lf %lf\n", n1, n2, n3);

//         n.x = n1; 
//         n.y = n2; 
//         n.z = n3;

//         line.erase();
//         getline(f, line);
//         //cout << line << endl;
//         if (line != "outer loop")
//         {
//             printf("%s should be equal 'outer loop'\n", line);
//             return -2;
//         }

//         line.erase();

//         SimpleFace SimpleFace;

//         for (int i = 0; i < 3; i++)
//         {
//             getline(f, line);
//             //cout << line << endl;
//             sscanf((line).c_str(), "%s%lf%lf%lf", str1,  &v[i].x, &v[i].y, &v[i].z);
//             v[i].faceId = count;
//             str1[0] = '\0';
//             SimpleFace.verts[i] = v[i];
//             line.erase();
//         }

//         SimpleFace.norm = n;
//         SimpleFaces.push_back(SimpleFace);

//         getline(f, line);
//         //cout << line << endl;
//         if (line != "endloop")
//         {
//             printf("%s should be equal 'endloop'\n", line);
//             return -2;
//         }
//         line.erase();
//         getline(f, line);
//         //cout << line << endl;
//         if (line != "endfacet")
//         {
//             printf("%s should be equal 'endfacet'\n", line);
//             return -2;
//         }
//         line.erase();

//         count++;
//         cout << count << endl;

//     }

//     return 0;
// }
