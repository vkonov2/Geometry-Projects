
#include "utils.h"

// перестановки

bool NextPermutation(int* a, int n)
{
    int j = n - 2;
    while (j != -1 && a[j] >= a[j + 1]) j--;
    if (j == -1) { return false; } // больше перестановок нет }
    int k = n - 1;
    while (a[j] >= a[k]) { k--; }
    Swap(a[j], a[k]);
    int l = j + 1, r = n - 1; // сортируем оставшуюся часть последовательности
    while (l < r) {
        Swap(a[l++], a[r--]);
    }
    return true;
}

// печать данных в файл

// template<class T>
void PrintToFile (vector<VECTOR2>& obj, int PointsCount, string& FILENAME, string& PATH)
{
    int i;
    string FullFileName;
    FullFileName = PATH + FILENAME;

    int size = int(obj.size());

    if (PointsCount > size)
    {
        cout << "\n>>> error in PrintToFile --> wrong input\n\n";
        return;
    }

    ofstream strm;

    strm.open(FullFileName.c_str());
    if (!strm.is_open())
    {
        cout << "\n>>> error in PrintToFile --> can't open file --> " << FullFileName << "\n\n";
        return;
    }

    for (i = 0; i < PointsCount; i++)
    {
        if (i == PointsCount-1)
            strm << obj[i].x << " " << obj[i].y;
        else
            strm << obj[i].x << " " << obj[i].y << "\n";
    }

    strm.close();
    if (strm.is_open())
    {
        cout << "\n>>> error in PrintToFile --> can't close file --> " << FullFileName << "\n\n";
        return ;
    }

    return ;
} 

double CalcAGM (double x, double y, double eps)
{
    double x_prev, x_cur;
    double y_prev, y_cur;
    double r_prev, r_cur;

    if (x < 0 || y < 0 || fabs(x) < eps || fabs(y) < eps || fabs(x+y) < eps)
    {
        cout << "\n>>> error in CalcAGM ---> wrong input\n\n";
        return -1.0;
    }

    x_prev = x;
    y_prev = y;
    r_prev = (x-y)/(x+y);

    while (true)
    {
        x_cur = (x_prev+y_prev)/2.0;
        y_cur = sqrt(x_prev*y_prev);
        r_cur = r_prev*r_prev/4.0;

        if (fabs(r_cur) < eps)
            break;

        x_prev = x_cur;
        y_prev = y_cur;
        r_prev = r_cur;
    }

    return x_cur;
}

double CalcMAGM (double x, double y, double eps)
{
    double x_prev, x_cur;
    double y_prev, y_cur;
    double z_prev, z_cur;
    double xi_prev, xi_cur;
    double eta_prev, eta_cur;
    double r_prev, r_cur;
    double rho_prev, rho_cur;
    double tmp;

    if (x < 0 || y < 0 || fabs(x) < eps || fabs(y) < eps || fabs(x+y) < eps)
    {
        cout << "\n>>> error in CalcAGM ---> wrong input\n\n";
        return -1.0;
    }

    x_prev = x;
    y_prev = y;
    z_prev = 0.0;

    r_prev = (x-y)/(x+y);

    xi_prev = x_prev - z_prev;
    eta_prev = y_prev - z_prev;
    rho_prev = (xi_prev+eta_prev)/(x_prev+y_prev);

    while (true)
    {
        x_cur = (x_prev+y_prev)/2.0;
        tmp = sqrt(xi_prev*eta_prev);
        y_cur = z_prev + tmp;
        z_cur = z_prev - tmp;

        r_cur = r_prev*r_prev/(4*rho_prev);

        if (fabs(r_cur) < eps)
            break;

        x_prev = x_cur;
        y_prev = y_cur;
        xi_prev = x_cur - z_cur;
        eta_prev = y_cur - z_cur;
        rho_prev = (xi_prev+eta_prev)/(x_cur+y_cur);
        r_prev = r_cur;
    }

    return x_cur;
}








