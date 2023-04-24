#define _USE_MATH_DEFINES

#include <iostream>
#include "Cluster.h"
#include <time.h>
// #include "../../ChainCreation/Gnuplot.h"

#include <mlpack/methods/dbscan/dbscan.hpp>
#include <mlpack/core.hpp>
#include <set>

using namespace std;
using namespace mlpack;
using namespace mlpack::dbscan;

size_t getClusters2(const arma::mat& points, const double epsilon, vector<vector<arma::mat>>& CLUSTERS);
void cart2pol(arma::mat& pt);
void pol2cart(arma::mat& pt);
double getQuarterDataAnglle(vector<arma::mat> quarterPoints, const double e);
int cluster_assigments(const arma::mat& points, const double epsilon, vector<int>& CLUSTERS);

size_t getClusters2(const arma::mat& points, const double epsilon, vector<vector<arma::mat>>& CLUSTERS)
{
    DBSCAN<> d(epsilon, 1);
    const int PointsCount = points.size() / 2;

    arma::Row<size_t> assigments;
    // clock_t start, end;

    const size_t ClusterCount = d.Cluster(points, assigments);
    // printf("ClusterCount = %d\n", ClusterCount);

    int ClusterId;
    CLUSTERS.resize(ClusterCount);

    for(int i = 0; i < PointsCount; i++)
    {
        arma::mat pt(2, 1);
        ClusterId = assigments[i];
        pt(0, 0) = points(0, i);
        pt(1, 0) = points(1, i);
        CLUSTERS[ClusterId].push_back(pt);
        pt.clear();
    }

    // for(int i = 0; i < ClusterCount; i++)
    // {
    //     const int n = CLUSTERS[i].size();
    //     printf("Cluster %d:\n");
    //     for (int j = 0; j < n; j++)
    //         printf("%lf %lf\n", CLUSTERS[i][j](0, 0), CLUSTERS[i][j](1, 0));
    // }

    assigments.clear();

    return ClusterCount;
}

int cluster_assigments(const arma::mat& points, const double epsilon, vector<int>& CLUSTERS)
{
    DBSCAN<> d(epsilon, 1);
    arma::Row<size_t> assigments;
    const size_t ClusterCount = d.Cluster(points, assigments);

    for(auto i: assigments)
        CLUSTERS.push_back(i);

    assigments.clear();

    return ClusterCount;
}

void cart2pol(arma::mat& pt)
{
    double x = pt(0, 0);
    double y = pt(1, 0);
    double rho = sqrt(x*x + y*y);
    double phi = atan2(y, x);
    pt(0, 0) = rho;
    // printf("B: %lf\n", phi);
    phi += 2 * M_PI;
    phi = fmod(phi, (2 * M_PI));
    // printf("A: %lf\n\n", phi);
    pt(1, 0) = phi;
}

void pol2cart(arma::mat& pt)
{
    double rho = pt(0, 0);
    double phi = pt(1, 0);
    double x = rho * cos(phi);
    double y = rho * sin(phi);
    pt(0, 0) = x;
    pt(1, 0) = y;
}

double getQuarterDataAnglle(vector<arma::mat> quarterPoints, const double e)
{    
    int n = int(quarterPoints.size());

    if (n == 0)
        return 0.0;
    
    for (int i = 0; i < n; i++)
        pol2cart(quarterPoints[i]);

    arma::mat points(2, n);

    for(int i = 0; i < n; ++i)
    {
        points(0, i) = quarterPoints[i](0, 0);
        points(1, i) = quarterPoints[i](1, 0);
    }

    vector<vector<arma::mat>> CLUSTERS;
    const int CLUSTERSsize = getClusters2(points, e, CLUSTERS);
    // DrawCluster(CLUSTERS);
        
    double angle_quarter = 0.0;

    for (int i = 0; i < CLUSTERSsize; i++)
    {
        vector<double> angles;
        n = int(CLUSTERS[i].size());

        if(n == 0)
            continue;

        for (int j = 0; j < n; j++)
        {
            cart2pol(CLUSTERS[i][j]);
            angles.push_back(CLUSTERS[i][j](1, 0));
        }

        // for (int j = 0; j < n; j++)
        //     printf("%lf\n", angles[j]);
        // printf("\n");    

        // printf("here start, CLUSTERS[i].size() = %d\n", n);
        auto maxA = max_element(begin(angles), end(angles));
        auto minA = min_element(begin(angles), end(angles));
        double part_angle = fabs((*maxA) - (*minA));
        // printf("here finish\n");
        angle_quarter += part_angle;

        // printf("minA = %lf, maxA = %lf, part_angle = %lf\n", minA, maxA, part_angle); 

        angles.clear();
    }

    for (int i = 0; i < CLUSTERSsize; i++)
        CLUSTERS[i].clear();
    
    CLUSTERS.clear();

    return angle_quarter;

}

struct comp {
  bool operator() (const arma::mat p1, arma::mat p2) { return p1(1, 0) < p2(1, 0); }
} cprArma;

double getClusterAngle(vector<arma::mat>& CLUSTER, const double e)
{
    vector<vector<arma::mat>> ptQuarts;
    ptQuarts.resize(4);
    sort(begin(CLUSTER), end(CLUSTER), cprArma);
    double angle = 0.0;

    for (auto p: CLUSTER)
    {
        if ((0.0 <= p(1, 0)) && (p(1, 0) <= M_PI / 2.0))
            ptQuarts[0].push_back(p);
        if ((M_PI / 2.0 <= p(1, 0)) && (p(1, 0) <= M_PI))
            ptQuarts[1].push_back(p);
        if ((M_PI <= p(1, 0)) && (p(1, 0) <= M_PI * (3.0 / 2.0)))
            ptQuarts[2].push_back(p);
        if ((M_PI * (3.0 / 2.0) <= p(1, 0)) && (p(1, 0) <= M_PI * 2.0))
            ptQuarts[3].push_back(p);
    }
    
    for (int i = 0; i < 4; i++)
    {
        double ret_angle = getQuarterDataAnglle(ptQuarts[i], e);
        const int n = ptQuarts[i].size();

        for (int j = 0; j < n; j++)
            ptQuarts[i][j].clear();
        ptQuarts[i].clear();
        
        angle += ret_angle;
    }

    ptQuarts.clear();
    
    return angle;
}
            

// double GirthC(const vector<VECTOR2>& pts)
double GirthC(const vector<vector<double>>& projection, const double epsilon)
{
    double angle = 0.0;
    const int ProjectionPointsCount = int(projection.size());

    if(ProjectionPointsCount == 0)
        return 0.0;
    
    arma::mat points(2, ProjectionPointsCount);

    for(int i = 0; i < ProjectionPointsCount; ++i)
    {
        points(0, i) = projection[i][0];
        points(1, i) = projection[i][1];
    }

    vector<vector<arma::mat>> CLUSTERS;
    getClusters2(points, epsilon, CLUSTERS);
    // DrawCluster(CLUSTERS);
    const int CLUSTERSsize = CLUSTERS.size();
    points.clear();

    for (int i = 0; i < CLUSTERSsize; i++)
    {
        const int n = CLUSTERS[i].size();

        for (int j = 0; j < n; j++)
            cart2pol(CLUSTERS[i][j]);

        angle += getClusterAngle(CLUSTERS[i], epsilon);
        
    }

    angle = (angle / M_PI) * 180;

    // printf("angle = %lf\n", angle);

    if (angle > 360.0)
        angle = 0.0;

    for (int i = 0; i < CLUSTERSsize; i++)
        CLUSTERS[i].clear();
    
    CLUSTERS.clear();

    // printf("angle = %lf\n", angle);

    return angle;
}

int norm_clusters(const vector<vector<double>>& norms, const double epsilon, vector<int>& CLUSTERS)
{
    const int PointsCount = int(norms.size());

    if(PointsCount == 0)
        return 0;
    
    arma::mat points(3, PointsCount);

    for(int i = 0; i < PointsCount; ++i)
    {
        points(0, i) = norms[i][0];
        points(1, i) = norms[i][1];
        points(2, i) = norms[i][2];
    }

    const int ClusterCount = cluster_assigments(points, epsilon, CLUSTERS);
    points.clear();

    return ClusterCount;
}