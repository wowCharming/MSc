#include <iostream>
#include <fstream>  
#include <sstream>  
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
	Matrix<T, ROWS, COLS>()
	{
		
		static_assert(ROWS * COLS >= 0, "length of vector less than 0");
		data_ = new T[ROWS * COLS];
		for (int i = 0; i < ROWS * COLS; i++)
		{
			data_[i] = 0.0;
		}
	}


	Matrix<T, ROWS, COLS>(T initialValue)
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

	//really important
	Matrix<T, ROWS, COLS>(const Matrix & m)
	{
		std::copy(m.data_.begin(), m.data_.end(), data_.begin());
	}


	~Matrix()
	{
		//delete[] data_;
	};
	

	T operator() (int i, int j) const
	{
		assert(i >= 0 && i < ROWS && j >= 0 && j < COLS);
		return data_[i * COLS + j];
	}

	T & operator() (int i, int j)
	{
		assert(i >= 0 && i < ROWS && j >= 0 && j < COLS);
		return data_[i * COLS + j];
	}

	Matrix<T, ROWS, COLS> & operator= (const Matrix<T, ROWS, COLS> & m);
	Matrix<T, ROWS, COLS> operator+(const Matrix<T, ROWS, COLS> & m);
	Matrix<T, ROWS, COLS> operator-(const Matrix<T, ROWS, COLS> & m);
	Matrix<T, ROWS, COLS> operator*(const Matrix<T, ROWS, COLS> & m);
	//Vector len?
	Vector<T, COLS> operator*(const Vector<T, COLS> & v)const;

	Matrix<T, ROWS, COLS> operator+=(const Matrix<T, ROWS, COLS> & m);
	Matrix<T, ROWS, COLS> operator-=(const Matrix<T, ROWS, COLS> & m);
	Matrix<T, ROWS, COLS> operator*=(const Matrix<T, ROWS, COLS> & m);
	bool operator==(const Matrix<T, ROWS, COLS> & m);
	bool operator!=(const Matrix<T, ROWS, COLS> & m);
	Matrix<T, ROWS, COLS> inverseDiagonal()const;

private:
	//int row;
	//int col;
	std::array<T, ROWS*COLS> data_;
	//T * data_;

};

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> &Matrix<T, ROWS, COLS>::operator= (const Matrix<T, ROWS, COLS> & m)
{
	Matrix<T, ROWS, COLS> tmp(m);

	std::swap(ROWS, tmp.ROWS);
	std::swap(COLS, tmp.COLS);
	std::swap(data_, tmp.data_);
	
	return *this;
}

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator+ (const Matrix<T, ROWS, COLS> & m)
{
	Matrix<T, ROWS, COLS> m0(0.0);
	static_assert(this->ROWS == m.ROWS && COLS == m.COLS, "2 matrix not equal");
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			m0.data_[i * COLS + j] = data_[i * COLS + j] + m.data_[i * COLS + j];//left use m0
		}
	}

	return m0;
};

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator- (const Matrix<T, ROWS, COLS> & m)
{
	Matrix<T, ROWS, COLS> m0(0);
	static_assert(this->ROWS == m.ROWS && COLS == m.COLS, "2 matrix not equal");

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			m0.data_[i * COLS + j] = data_[i * COLS + j] - m.data_[i * COLS + j];//left use m0
																				 //m0.GetData(i, j) = *this.GetData(i, j) - m.GetData(i, j);
		}
	}

	return m0;
};

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator* (const Matrix<T, ROWS, COLS> & m)
{
	Matrix<T, ROWS, COLS> m0(0);

	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < m.COLS; ++j)
		{
			double sum = 0.0;
			for (int p = 0; p < COLS; ++p)
			{
				sum += data_[i * COLS + p] * m.data_[p * m.COLS + j];//wrong: m.data_[p * col + j]
			}
			m0.data_[i * m.COLS + j] = sum; //correction:i * m.col + j
											//m0(i, j) = sum;//correction here!!!!
		}
	}

	return m0;
};

//matrix * vector 
template <class T, size_t ROWS, size_t COLS>//, size_t LEN
Vector<T, COLS> Matrix<T, ROWS, COLS>::operator* (const Vector<T, COLS> & v) const
{
	Vector<T, COLS> v0(0);

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
};

template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator+= (const Matrix<T, ROWS, COLS> & m)
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
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator-= (const Matrix<T, ROWS, COLS> & m)
{

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS; j++)
		{
			data_[i*COLS + j] = m.data_[i*COLS + j] - data_[i*COLS + j];
		}
	}

	return  *this;
}

//m*n(i*p) & n*n(p*j)
template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::operator*= (const Matrix<T, ROWS, COLS> & m)
{
	Matrix m0(0);

	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < m.COLS; j++)
		{
			double sum = 0;
			for (int p = 0; p < COLS; p++)
			{
				sum += data_[i * COLS + p] * m.data_[p * COLS + j];
			}
			m0(i, j) = sum;
		}
	}
	*this = m0;//add here to return the new this
	return  *this;
}

template <class T, size_t ROWS, size_t COLS>
bool Matrix<T, ROWS, COLS>::operator== (const Matrix<T, ROWS, COLS> & m)
{
	if (this->ROWS == m.ROWS && COLS == m.COLS)
	{
		for (int i = 0; i < ROWS; i++)
		{
			for (int j = 0; j < COLS; j++)
			{
				if (std::abs((i, j) - m(i, j)) > 1e-13)
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
bool Matrix<T, ROWS, COLS>::operator!= (const Matrix<T, ROWS, COLS> & m)
{
	return !(operator==(m));
}

//-----------------------------------------------
template <class T, size_t ROWS, size_t COLS>
Matrix<T, ROWS, COLS> Matrix<T, ROWS, COLS>::inverseDiagonal()const
{
	
	static_assert(ROWS == COLS, "matrix is not square");

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

