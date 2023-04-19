#ifndef _LEASTSQRT_H_
#define _LEASTSQRT_H_


double  TestPlane ( double *plane, int n, double *points, double *weights=0 );
double  ApproxByPlane0 ( double *plane, int n, double *points, double *weights=0 );
double  NearestPointToPlanes ( double *point, double *baseplane, int n, double *planes, double *weights=0 );
double  NearestPointToPlanes2 ( double *point, double *baseplane, int n, double *planes, double *weights=0 );
void TestNearestPointToPlanes2 ();


// ������������� �� �������� (������������)
double ApproxByPlane2 ( double *plane, int n, double *points, double *weights=0 );
double ApproxError ( double *plane, int n, double *points, double *weights=0 );



double ErrAproxByLine ( double * line, int n, double *points, double *weights=0 );

/*--------------------------------------------------------------------

  ���������� ������ ���������� ������������������� ����������� 
  ��� ��������� ��������� ����� ���������� ������������.

plane   - ������� ���������  
          X*plane[0] + Y*plane[1] + plane[2] = 0
n       - ���������� ���������������� �����
points  - ������ ��������� ����� (���� (x,y), ����� ������� 2*n)
weights - ������ ������� ������������� �����, ��� weights=0 ��� ���� 1

return  - ����������� ����������� �������������

--------------------------------------------------------------------*/

double ApproxByLine ( double * line, int n, double *points, double *weights =0  );




/*--------------------------------------------------------------------

  ���������� ������ ���������� ������������������� ����������� 
  ��� ��������� ��������� ����� ����������� ������������.

line    - ������� ������, �������� � ���� ����� (x,y,x) - line[0],line[1],line[2]
          � ���������� ������� ����������� (dx,dy,dz) - line[3],line[4],line[5]
n       - ���������� ���������������� �����
points  - ������ ��������� ����� (������ (x,y,z), ����� ������� 3*n)
weights - ������ ������� ������������� �����, ��� weights=0 ��� ���� 1

return  - ����������� ����������� �������������

--------------------------------------------------------------------*/
double ApproxByLine3D ( double * line, int n, double *points, double *weights=0 );
int TestApproxByLine3D ();

#endif // _LEASTSQRT_H_

