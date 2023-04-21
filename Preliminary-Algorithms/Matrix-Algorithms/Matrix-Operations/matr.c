#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print (double *a , int n, int m);
int step (int m, int n, double *a, double eps);
int power (double *a, int n, int pow);
void inverse (double *a, double *e, int n);
int det (double *a, int m, int n);

/*
    The program allows you to carry out various basic operations with matrices: reduction to an improved stepwise form, raising a matrix to a power, finding the inverse matrix, multiplying matrices, finding the determinant of a matrix
*/

void print (double *a , int n, int m)
{
    int i , j;
    printf ("=================================================================\n");
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
            printf("%10.3lf ", a[i*n + j]);
        
        printf("\n");
    }
    printf ("=================================================================\n");
    return ;
}

int step (int m , int n, double *a, double eps)
{
    int i = 0, j = 0, k, l, rank;
    int *x;
    double val;
    
    x = (int*) malloc (n * sizeof(int));
    if (!x)
    {
        printf ("Allocation error\n");
        return -1;
    }
    
    while (i < m && j < n)
    {
        val = 0.0;
        
        for (k = i; k < m; k++)
        {
            if (fabs(a[k*n + j]) > val)
            {
                l = k;
                val = fabs(a[k*n + j]);
            }
        }
        
        if (val <= eps)
        {
            for (k = i; k < m; k++)
                a[k*n + j] = 0.0;
            
            j++;
            continue;
        }
        
        if (l != i)
        {
            for (k = j; k < n; k++)
            {
                val = a[i*n + k];
                a[i*n + k] = a[l*n + k];
                a[l*n + k] = val;
            }
        }
        
        for (k = i+1; k < m; k++)
        {
            val = (-a[k*n + j] / a[i*n + j]);
            
            a[k*n + j] = 0.0;
            
            for (l = j+1; l < n; l++)
                a[k*n + l] += val * a[i*n + l];
        }
        
        i++;
        j++;
    }
    
    //print (a , n, m);
    
    rank = i;
    //printf ("rank = %d\n", rank);
    i = 0; j = 0; val = 0; k = 0;
    
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n-1; j++)
        {
            if ((fabs(a[i*n + j] - 0) < 1e-15) && (fabs(a[i*n + (j+1)] - 0) < 1e-15))
                k++;
        }
        
        x[i] = k+2;
        k = 0;
    }
    
    x[0] = 1;
    k = 0;
    
    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (fabs(a[i*n + j] - 0) < 1e-15)
                k++;
        }
        
        //printf ("k[%d] = %d\n", i, k);
        
        if (k == n)
            x[i] = 0;
        
        k = 0;
    }
    
    /*for (i = 0; i < n; i++)
     printf ("x[%d] = %d\n", i, x[i]);*/
    
    val = a[0];
    
    for (i = 0; i < m-1; i++)
    {
        //printf ("val[%d] = %lf\n", i, val);
        
        for (j = x[i]-1; j < n; j++)
            a[i*n + j] = a[i*n + j] / val;
        
        //printf ("a[%d] = %lf\n", (i+1)*n + n-1, a[(i+1)*n + n-1]);
        
        if (x[i+1] == n)
            val = a[(i+1)*n + n-1];
        else
            val = a[(i+1)*n + x[i+1]-1];
    }
    
    for (j = x[m-1]-1; j < n; j++)
        a[(m-1)*n + j] = a[(m-1)*n + j] / val;
    
    //print (a , n , m);
    
    for (i = rank-1; i >= 0; i--)
    {
        for (k = 0; k < i; k++)
        {
            val = a[k*n + x[i]-1];
            
            //printf ("val[%d] = %lf\n", i, val);
            
            for (j = x[i]-1; j < n; j++)
                a[k*n + j] = a[k*n + j] - a[i*n + j] * val;
        }
    }
    
    print (a , n , m);
    
    free (x);
    return 1;
}

int power (double *a, int n, int pow)
{
    double *x, *y;
    double val = 0;
    int i, j, k, l = 0;
    
    x = (double *) malloc(n * n * sizeof(double));
    if (!x)
    {
        printf ("Allocation error\n");
        return -2;
    }
    
    y = (double *) malloc(n * n * sizeof(double));
    if (!y)
    {
        printf ("Allocation error\n");
        return -2;
    }
    
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
            x[i*n + j] = a[i*n + j];
    }
    
   while (l < pow-1)
   {
       for (i = 0; i < n; i++)
       {
           for (j = 0; j < n; j++)
           {
               for (k = 0; k < n; k++)
                   val += x[i*n + k] * a[k*n + j];
               
               y[i*n + j] = val;
               val = 0;
           }
       }
       l++;
       
       for (i = 0; i < n; i++)
       {
           for (j = 0; j < n; j++)
               x[i*n + j] = y[i*n + j];
       }
   }
    
    print (y, n, n);
    
    free(x);
    free (y);
    return 1;
}

void inverse (double *a, double *e, int n)
{
    int string = 0, num = -1, i, j, l;
    double val, k;
    
    for (j = 0; j < n; j++)
    {
        for (i = string; i < n; i++)
        {
            if (a[i*n + j] != 0)
            {
                num = i;
                break;
            }
        }
        
        if (num > -1)
        {
            val = a[string*n + j];
            
            for (i = 0; i < string; i++)
            {
                k = a[i*n + j] / val;
                
                for (l = 0; l < n; l++)
                {
                    a[i*n + l] -= k * a[string*n + l];
                    
                    e[i*n + l] -= k * e[string*n + l];
                }
            }
            
            for (i = string+1; i < n; i++)
            {
                k = a[i*n + j] / val;
                
                for (l = 0; l < n; l++)
                {
                    a[i*n + l] -= k * a[string*n + l];
                    
                    e[i*n + l] -= k * e[string*n + l];
                }
            }
            
            string += 1;
        }
        
        if (num == -1)
        {
            printf("Inverse matrix does not exist\n");
            
            return ;
        }
    }
    
    k = 0;
    
    for (i = 0; i < n; i++)
    {
        if (fabs (a[i*n + i] - 0) > 1e-15)
        {
            k = 1/a[i*n + i];
            
            a[i*n + i] = 1;
            
            for (j = 0; j < n; j++)
                e[i*n + j] *= k;
        }
        else
        {
            printf("Inverse matrix does not exist\n");
            
            return ;
        }
    }
    
    print (e, n, n);
    
    return ;
}

int det (double *a, int m, int n)
{
    int i = 0, j = 0, k, l, rank;
    int *x;
    double val, det = 1;
    
    x = (int*) malloc (n * sizeof(int));
    if (!x)
    {
        printf ("Allocation error\n");
        return -1;
    }
    
    while (i < m && j < n)
    {
        val = 0.0;
        
        for (k = i; k < m; k++)
        {
            if (fabs(a[k*n + j]) > val)
            {
                l = k;
                val = fabs(a[k*n + j]);
            }
        }
        
        if (val <= 1e-15)
        {
            for (k = i; k < m; k++)
                a[k*n + j] = 0.0;
            
            j++;
            continue;
        }
        
        if (l != i)
        {
            for (k = j; k < n; k++)
            {
                val = a[i*n + k];
                a[i*n + k] = a[l*n + k];
                a[l*n + k] = val;
            }
        }
        
        for (k = i+1; k < m; k++)
        {
            val = (-a[k*n + j] / a[i*n + j]);
            
            a[k*n + j] = 0.0;
            
            for (l = j+1; l < n; l++)
                a[k*n + l] += val * a[i*n + l];
        }
        
        i++;
        j++;
    }
    
    //print (a , n, m);
    
    rank = i;
    //printf ("rank = %d\n", rank);
    i = 0; j = 0; val = 0; k = 0;
    
    if (rank == n)
    {
        for (i = 0; i < n; i++)
            det *= a[i*n + i];
        
        printf ("Определитель равен: %10.3lf\n", det);
    }
    else
    {
        det = 0;
        printf ("Определитель равен: %10.3lf\n", det);
    }
    
    return 1;
}

int main(void)
{
    int m, n, i, j, k, val, pow, s, string = 0, num, p, q;
    double *a, *b, *c, *e;
    
    printf ("Введите 0, если хотите привести матрицу к улучшенному ступенчатому виду\n");
    printf ("Введите 1, если хотите возвести матрицу в степень\n");
    printf ("Введите 2, если хотите найти обратную матрицу\n");
    printf ("Введите 3, если хотите перемножить матрицы\n");
    printf ("Введите 4, если хотите найти определитель матрицы\n");
    
    if (scanf("%d", &s) != 1)
    {
        printf ("Entered wrong data\n");
        return -1;
    }
    
    if (s == 0)
    {
        printf("Введите количество строк матрицы: ");
        if (scanf("%d", &m) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        printf("Введите количество столбцов матрицы: ");
        if (scanf("%d", &n) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        a = (double *) malloc(m * n * sizeof(double));
        if (!a)
        {
            printf ("Allocation error\n");
            return -2;
        }
        
        printf("Введите элементы матрицы:\n");
        
        for (i = 0; i < m; i++)
        {
            for (j = 0; j < n; j++)
            {
                val = i*n + j;
                printf ("a[%d] = ", val);
                if (scanf("%lf", &a[i*n + j]) != 1)
                {
                    printf ("Entered wrong data\n");
                    return -1;
                }
            }
        }
        
        print (a , n , m);
        
        step (m, n, a, 10e-14);
        
        free (a);
    }
    
    if (s == 1)
    {
        printf("Введите размер матрицы коэффициентов: ");
        if (scanf("%d", &n) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        a = (double *) malloc(n * n * sizeof(double));
        if (!a)
        {
            printf ("Allocation error\n");
            return -2;
        }
        
        printf("Введите элементы матрицы коэффициентов:\n");
        
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                val = i*n + j;
                printf ("a[%d] = ", val);
                if (scanf("%lf", &a[i*n + j]) != 1)
                {
                    printf ("Entered wrong data\n");
                    return -1;
                }
            }
        }
        
        printf ("Введите необходимую степень:\n");
        if (scanf("%d", &pow) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        print (a , n, n);
        
        power (a, n, pow);
        
        free (a);
    }
    
    if (s == 2)
    {
        printf("Введите размер матрицы коэффициентов: ");
        if (scanf("%d", &n) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        a = (double*)malloc(n*n*sizeof(double));
        if (!a)
        {
            printf ("Allocation error\n");
            return -2;
        }
        
        e = (double*)malloc(n*n*sizeof(double));
        if (!e)
        {
            printf ("Allocation error\n");
            return -2;
        }
        
        printf("Введите элементы матрицы коэффициентов:\n");
        
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                val = i*n + j;
                printf ("a[%d] = ", val);
                if (scanf("%lf", &a[i*n + j]) != 1)
                {
                    printf ("Entered wrong data\n");
                    return -1;
                }
            }
        }
        
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                if (i == j)
                    e[i*n + j] = 1;
                else
                    e[i*n + j] = 0;
            }
        }
        
        print (a, n, n);
        
        inverse (a, e, n);
        
        free (a);
        free (e);
    }
    
    if (s == 3)
    {
        printf("Введите количество строк матрицы A: ");
        if (scanf("%d", &m) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        printf("Введите количество столбцов матрицы A: ");
        if (scanf("%d", &n) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        printf("Введите количество строк матрицы B: ");
        if (scanf("%d", &p) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        printf("Введите количество столбцов матрицы B: ");
        if (scanf("%d", &q) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        if (n == p)
        {
            a = (double *) malloc(m * n * sizeof(double));
            if (!a)
            {
                printf ("Allocation error\n");
                return -2;
            }
            
            printf("Введите элементы матрицы A:\n");
            
            for (i = 0; i < m; i++)
            {
                for (j = 0; j < n; j++)
                {
                    val = i*n + j;
                    printf ("a[%d] = ", val);
                    if (scanf("%lf", &a[i*n + j]) != 1)
                    {
                        printf ("Entered wrong data\n");
                        return -1;
                    }
                }
            }
            
            b = (double *) malloc(n * q * sizeof(double));
            if (!b)
            {
                printf ("Allocation error\n");
                return -2;
            }
            
            printf("Введите элементы матрицы B:\n");
            
            for (i = 0; i < n; i++)
            {
                for (j = 0; j < q; j++)
                {
                    val = i*p + j;
                    printf ("b[%d] = ", val);
                    if (scanf("%lf", &b[i*p + j]) != 1)
                    {
                        printf ("Entered wrong data\n");
                        return -1;
                    }
                }
            }
            
            print (a , n , m);
            print (b , q , n);
            
            c = (double *) malloc(m * q * sizeof(double));
            if (!c)
            {
                printf ("Allocation error\n");
                return -2;
            }
            
            for (i = 0; i < m; i++)
            {
                for (j = 0; j < q; j++)
                    c[i*q + j] = 0;
            }
            
            //print (c, q, m);
            
            for (i = 0; i < m; i++)
            {
                for (j = 0; j < q; j++)
                {
                    for(k = 0; k < n; k++)
                        c[i*q + j] += a[i*n + k] * b[k*p + j];
                }
            }
            
            print (c, q, m);
            
            free (a);
            free (b);
            free (c);
        }
        else
            printf ("Matrices can not be multiplied\n");
    }
    
    if (s == 4)
    {
        printf("Введите размер матрицы коэффициентов: ");
        if (scanf("%d", &n) != 1)
        {
            printf ("Entered wrong data\n");
            return -1;
        }
        
        a = (double *) malloc(n * n * sizeof(double));
        if (!a)
        {
            printf ("Allocation error\n");
            return -2;
        }
        
        printf("Введите элементы матрицы коэффициентов:\n");
        
        for (i = 0; i < n; i++)
        {
            for (j = 0; j < n; j++)
            {
                val = i*n + j;
                printf ("a[%d] = ", val);
                if (scanf("%lf", &a[i*n + j]) != 1)
                {
                    printf ("Entered wrong data\n");
                    return -1;
                }
            }
        }
        
        print (a, n, n);
        
        det (a, n, n);
        
        free(a);
    }
    
    return 0;
}



