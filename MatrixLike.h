#pragma once

using namespace std;


// forward declarations
template<typename T, size_t LEN>
class Vector;
//template<typename T, size_t ROWS, size_t COLS>
//class Matrix;

template<typename T, class Derived, size_t ROWS, size_t COLS>
class MatrixLike {
public:
	/// c'tor/ d'tor
	virtual ~MatrixLike ( ) noexcept = 0; // pure virtual destructor

	/// virtual operators
	virtual Vector<T, ROWS> operator* (const Vector<T, COLS> & o) const = 0;
	//virtual Matrix<T, ROWS, 1> operator* (const Matrix<T, COLS, 1> & o) const = 0;

	// feel free to extend as required

	// TODO: optimize the () operator
	//virtual const T & operator() (int r, int c) const = 0;
	//virtual T & operator() (int r, int c) = 0;

	/// other functions 
	virtual Derived inverseDiagonal( ) const = 0;
	// feel free to extend as required

protected:
};

// MatrixLike d'tor implementation
template<typename T, class Derived, size_t ROWS, size_t COLS>
inline MatrixLike<T, Derived, ROWS, COLS>::~MatrixLike ( ) noexcept { }
