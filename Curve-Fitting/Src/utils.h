
#ifndef UTILS_H_
#define UTILS_H_

// здесь собраны процедуры общего характера для всяких мелких операций 

// обмен значений

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include "Geometry/Vector2.h"

using namespace std;

template <class T>
inline void Swap(T &a, T &b) 
{
    T tmp = a; a = b; b = tmp;
}

// знак выражения

template <typename T> 
inline int sgn(T &val) 
{
    return (T(0) < val) - (val < T(0));
}

// экстремумы

#ifndef max 
    #define max(a,b)  (((a)>(b)) ? (a) : (b))
#endif

#ifndef min 
    #define min(a,b)  (((a)<(b)) ? (a) : (b))
#endif

// перестановки

bool NextPermutation(int* a, int n);

// нормировка объекта

template <class T>
inline void Normalize(T &obj)
{
    int size = (int)(obj.size());
    double val, norm = 0.0;
    for (int i = 0; i < size; i++)
    {
        val = obj[i];
        norm += val*val;
    }
    norm = sqrt(norm);
    for (int i = 0; i < size; i++)
        obj[i] /= norm;
}

// печать данных в файл

// template<class T>
void PrintToFile (vector<VECTOR2>& obj, int PointsCount, string& FILENAME, string& PATH);

double CalcMAGM (double x, double y, double eps);
double CalcAGM (double x, double y, double eps);

#endif // UTILS_H_


