
// #include "Matrix.h"
// #include "Format.h"

// void PrintMatrix(const string& sText, ublas::matrix<double>& a)
// {
//     cout<<sText<<endl;
// 	string row;
// 	BOOST_FOREACH(int i, irange(0, (int)(a.size1())))
// 	{
// 		row = "";
// 		BOOST_FOREACH(int j, irange(0, (int)(a.size2())))
// 			row += str(format("%1% ") % a(i, j));
// 		cout << row << endl;
// 	}
// 	cout << endl;
// }

// bool InverseMatrix ( const ublas::matrix<double>& input, ublas::matrix<double>& inverse)
// {
// 	typedef ublas::permutation_matrix<double> pmatrix;

// 	ublas::matrix<double> A(input);
// 	pmatrix pm(A.size1());

// 	int res = lu_factorize(A,pm);
// 		if( res != 0 ) return false;

// 	inverse.assign(ublas::identity_matrix<double>(A.size1()));
	
// 	lu_substitute(A, pm, inverse);

// 	return true;
// }

// int main (void)
// {
// 	ublas::matrix<double> A(2, 2);
// 	ublas::matrix<double> invA(2, 2);
	
// 	A(0,0) = 1.0;
// 	A(0,1) = 2.0;
// 	A(1,0) = 3.0;
// 	A(1,1) = 4.0;
	
// 	InverseMatrix(A, invA);

// 	cout << A << endl;

// 	cout << invA << endl;

// 	cout << conj(A) << endl;

// 	return 1;
// }


