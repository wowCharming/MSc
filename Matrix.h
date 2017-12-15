#include <iostream>
#include <fstream>  
#include <sstream>  
#include <istream>
#include <string>
#include <vector>  
#include <cassert>
#include <algorithm>
#include <array>
#include "MatrixLike.h"

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::istream;
using std::ostream;
using std::endl;

template <typename T, size_t ROWS, size_t COLS>
class Matrix : public MatrixLike <T, Matrix<T, ROWS, COLS>, ROWS, COLS>
{
public:
	Matrix() : row(ROWS), col(COLS)
	{
		
		static_assert(ROWS * COLS >= 0, "length of vector less than 0");
		data_ = new T[ROWS * COLS];
		for (int i = 0; i < ROWS * COLS; i++)
		{
			data_[i] = 0.0;
		}
	}


	Matrix(T initialValue) : row(ROWS), col(COLS)
	{
		static_assert(ROWS * COLS >= 0, "length of vector less than 0");
		
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLS; j++)
			{
				data_[i * COLS + j] = initialValue;
			}
		}
	}
	/*Matrix(T init) : row(ROWS), col(COLS), (std::array<T, ROWS*COLS>())
	{
		static_assert(ROWS > 0 && COLS > 0, "length of vector less than 0");
		std::fill(data_.begin(), data_.end(), init);
	}*/

	//really important
	Matrix(const Matrix & m) : row(m.row), col(m.col)//, data_(std::array<T, ROWS*COLS>())
	{
		data_ = array<T, ROWS*COLS>();//very important
		std::copy(m.data_.begin(), m.data_.end(), data_.begin());
	}

	~Matrix()
	{
		//delete[] data_;
	}
	

	const T & operator() (int i, int j) const
	{
		assert(i >= 0 && i < ROWS && j >= 0 && j < COLS);
		return data_[i * COLS + j];
	}

	T & operator() (int i, int j)
	{
		assert(i >= 0 && i < ROWS && j >= 0 && j < COLS);
		return data_[i * COLS + j];
	}

	Matrix & operator= (const Matrix & m);
	Matrix operator+(const Matrix & m) const;
	Matrix operator-(const Matrix & m) const;

	template<size_t N>//need to specified that to take care about the size of the matrix
	Matrix<T, ROWS, N> operator*(const Matrix<T, COLS, N> & m) const;
	//Vector len?
	Vector<T, ROWS> operator*(const Vector<T, COLS> & v) const;
	//Matrix<T, ROWS, 1> operator* (const Matrix<T, COLS, 1> & v) const;
	
	Matrix & operator+=(const Matrix & m);
	Matrix & operator-=(const Matrix & m);
	template<size_t N>
	Matrix<T, ROWS, N> & operator*=(const Matrix<T, COLS, N> & m);
	bool operator==(const Matrix & m) const;// const to be read only
	bool operator!=(const Matrix & m) const;
	template<size_t M, size_t N>// allow comparison of matrices of different dimensions
	bool operator==(const Matrix<T, M, N> & m) const;
	template<size_t M, size_t N>
	bool operator!=(const Matrix<T, M, N> & m) const;

	Matrix inverseDiagonal() const;

private:
	int row;
	int col;
	std::array<T, ROWS*COLS> data_;
	//T * data_;

};

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> &Matrix<T, ROWS, COLS>::operator= (const Matrix<T, ROWS, COLS> & m)
{
	Matrix<T, ROWS, COLS> tmp(m);

	std::swap(row, tmp.row);
	std::swap(col, tmp.col);
	std::swap(data_, tmp.data_);
	
	return *this;
}

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator+ (const Matrix<T, ROWS, COLS> & m) const
{
	Matrix<T, ROWS, COLS> m0(0.0);
	//static_assert(this->row == m.row && col == m.col, "2 matrix not equal");
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			m0.data_[i * COLS + j] = data_[i * COLS + j] + m.data_[i * COLS + j];//left use m0
		}
	}

	return m0;
}

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator- (const Matrix<T, ROWS, COLS> & m) const
{
	Matrix<T, ROWS, COLS> m0(0);
//	static_assert(this->row == m.row && col == m.col, "2 matrix not equal");

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			m0.data_[i * COLS + j] = data_[i * COLS + j] - m.data_[i * COLS + j];//left use m0
																				 //m0.GetData(i, j) = *this.GetData(i, j) - m.GetData(i, j);
		}
	}

	return m0;
}

template <class T, size_t ROWS, size_t COLS>
template<size_t N>
Matrix<T, ROWS, N> Matrix<T, ROWS, COLS>::operator* (const Matrix<T, COLS, N> & m) const
{
	Matrix<T, ROWS, N> m0(0);

	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			double sum = 0.0;
			for (int p = 0; p < COLS; ++p)
			{
				//sum += data_[i * COLS + p] * m.data_[p * N + j];//wrong: m.data_[p * col + j]
				sum += (*this)(i, p) * m(p, j);
			}
			m0(i,j) = sum; //correction:i * m.col + j
						   //not: m0.data_[i * col + j] = sum;//correction here!!!!
		}
	}

	return m0;
}

//matrix * vector 
template <class T, size_t ROWS, size_t COLS>//, size_t LEN
Vector<T, ROWS> Matrix<T, ROWS, COLS>::operator* (const Vector<T, COLS> & v) const
{
	Vector<T, ROWS> v0(0);

	for (int i = 0; i < ROWS; ++i)
	{
		T sum = 0.0;
		for (int j = 0; j < COLS; ++j)
		{
			sum += data_[i*COLS + j] * v.GetData(j);
		}
		v0(i) = sum;
	}

	return v0;
}
//template <class T, size_t ROWS, size_t COLS>
//Matrix<T, ROWS, 1> Matrix<T, ROWS, COLS>::operator* (const Matrix<T, COLS, 1> & v) const
//{
//	Matrix<T, ROWS, 1> v0(0);
//
//	for (int i = 0; i < ROWS; ++i)
//	{
//		T sum = 0.0;
//		for (int j = 0; j < COLS; ++j)
//		{
//			sum += data_[i*COLS + j] * v.GetData(j);
//		}
//		v0(i) = sum;
//	}
//
//	return v0;
//}

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> & Matrix<T, ROWS, COLS>::operator+= (const Matrix<T, ROWS, COLS> & m)
{

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			data_[i*COLS + j] = m.data_[i*COLS + j] + data_[i*COLS + j];
		}
	}

	return  *this;
}

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> & Matrix<T, ROWS, COLS>::operator-= (const Matrix<T, ROWS, COLS> & m)
{

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			data_[i*COLS + j] = data_[i*COLS + j] - m.data_[i*COLS + j];
		}
	}

	return  *this;
}

//m*n(i*p) & n*n(p*j)
template <class T, size_t ROWS, size_t COLS>
template<size_t N>
Matrix<T, ROWS, N> & Matrix<T, ROWS, COLS>::operator*= (const Matrix<T, COLS, N> & m)
{
	//add here to return the new this
	return  (*this) = (*this) * m;
}

template <class T, size_t ROWS, size_t COLS>
bool Matrix<T, ROWS, COLS>::operator== (const Matrix<T, ROWS, COLS> & m) const
{
	if (this->row == m.row && col == m.col)
	{
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLS; j++)
			{
				if (data_[i * COLS + j] != m.data_[i * COLS + j])
					//(std::abs((i, j) - m(i, j)) > 1e-13)
				{
					return false;
				}
			}
		}
		return true;

	}
	else
	{
		return false;
	}
}

template <class T, size_t ROWS, size_t COLS>
bool Matrix<T, ROWS, COLS>::operator!= (const Matrix<T, ROWS, COLS> & m) const
{
	return !(operator==(m));
}

template <class T, size_t ROWS, size_t COLS>
template<size_t M, size_t N>
bool Matrix<T, ROWS, COLS>::operator== (const Matrix<T,	M, N> & m) const
{
	if (this->row == M && col == N)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <class T, size_t ROWS, size_t COLS>
template<size_t M, size_t N>
bool Matrix<T, ROWS, COLS>::operator!=(const Matrix<T, M, N> & m) const
{
	return !(operator==(m));
}

//-----------------------------------------------
template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::inverseDiagonal()const
{
	
//	static_assert(row == col, "matrix is not square");

	Matrix<T, ROWS, COLS> m(0.0);

	T temp = 0;
	for (int i = 0; i< ROWS; i++)
	{
		temp = 1 / data_[i*COLS + i];
		m(i, i) = temp;
	}

	return m;
}

template <class T, size_t ROWS, size_t COLS>
std::ostream & operator<<(std::ostream & os, const Matrix<T, ROWS, COLS> & m)
{
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			os << m(i, j) << " "; //no comma
		}
		os << endl;
	}
	return os;
}

template <class T, size_t ROWS, size_t COLS>
std::istream & operator >> (std::istream& is, Matrix<T, ROWS, COLS> & m)
{
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j)
		{
			is >> m(i, j);
		}
	}
	return is;
}
