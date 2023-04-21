#ifndef FUNCTIONS_H 
#define FUNCTIONS_H 

int solution_alloc (int n, int m, double eps, int k, const char* filename);

int solve (int n, int m, double eps, double *A);

int yakobi (double *A, int n, int i, int j);

double norm (int n, int k, const char* filename, double* A);

void print (double *A , int l, int n, int m);

double formula (int k, int n, int i, int j);

double time_now(void);

#endif
