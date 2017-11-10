#pragma once
#include <iostream>
#include <fstream>  
#include <sstream>  
#include <vector>  
#include <cassert>
#include <algorithm>
#include <assert.h>

using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::istream;
using std::ostream;
using std::endl;


class Matrix
{
public:
	Matrix(int r, int c) : row(r), col(c)
	{
		int size = row * col;
		assert(size >= 0);
		data_ = new double[size];
	}


	Matrix(int r, int c, double initialValue) : row(r), col(c)
	{
		int size = row * col;
		assert(size >= 0);
		data_ = new double[size];

		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				data_[i * col + j] = initialValue;
			}
		}
	}

	Matrix(const Matrix & m)
	{
		int size = m.row * m.col;
		data_ = new double[size];
		std::copy(m.data_, m.data_ + size, data_);

		//do not work
		/*for (int i = 0; i < m.row; i++)
		{
			for (int j = 0; j < m.col; j++)
			{
				data_[i * col + j] = m.data_[i * col + j];
			}
		}*/
	}


	~Matrix()
	{
		delete [] data_;
	};

	int getColumns() const
	{
		return col;
	}

	int getRows() const
	{
		return row;
	}

	double operator() (int i, int j) const
	{
		assert(i >= 0 && i < row && j >= 0 && j < col);
		return data_[i * col + j];
	}

	double & operator() (int i, int j)
	{
		assert(i >= 0 && i < row && j >= 0 && j < col);
		return data_[i * col + j];
	}


	Matrix & operator= (const Matrix & m);
	Matrix operator+(const Matrix & m);
	Matrix operator-(const Matrix & m);
	Matrix operator*(const Matrix & m);
	Matrix operator+=(const Matrix & m);
	Matrix operator-=(const Matrix & m);
	Matrix operator*=(const Matrix & m);
	bool operator==(const Matrix & m);
	bool operator!=(const Matrix & m);

private:
	int row;
	int col;
	//int currentRow;

	double * data_;

};

Matrix &Matrix::operator= (const Matrix & m)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			data_[i * col + j] = m.data_[i * col + j];
		}
	}
	return *this;
}


Matrix Matrix::operator+ (const Matrix & m)
{
	Matrix m0(row, col);//if ini val
	assert(this->row == m.row && col == m.col);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			m0.data_[i * col + j] = data_[i * col + j] + m.data_[i * col + j];//left use m0
		}
	}

	return m0;
}

Matrix Matrix::operator- (const Matrix & m)
{
	Matrix m0(row, col);
	assert(this->row == m.row && col == m.col);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			m0.data_[i * col + j] = data_[i * col + j] - m.data_[i * col + j];//left use m0
		}
	}

	return m0;
}

Matrix Matrix::operator* (const Matrix & m)
{
	assert(this->col == m.row);
	Matrix m0(row, m.col);

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < m.col; ++j)
		{
			double sum = 0.0;
			for (int p = 0; p < col; ++p)
			{
				sum += data_[i * col + p] * m.data_[p * m.col + j];//problem here: m.col
			}
			//m0.data_[i * m.col + j] = sum; //correction:i * m.col + j
			m0(i, j) = sum;//correction here!!!!
		}
	}

	return m0;
}

Matrix Matrix::operator+= (const Matrix & m)
{
	assert(this->row == m.row && col == m.col);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			data_[i*col + j] = m.data_[i*col + j] + data_[i*col + j];
		}
	}

	return  *this;
}

Matrix Matrix::operator-= (const Matrix & m)
{
	assert(this->row == m.row && col == m.col);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			data_[i*col + j] = m.data_[i*col + j] - data_[i*col + j];
		}
	}

	return  *this;
}

//m*n(i*p) & n*n(p*j)
Matrix Matrix::operator*= (const Matrix & m)
{
	assert(this->col == m.row);
	Matrix m0(row, m.col);

	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < m.col; j++)
		{
			double sum = 0;
			for (int p = 0; p < col; p++)
			{
				sum += data_[i * col + p] * m.data_[p * col + j];
			}
			//data_[i * col + j] = sum;
			m0(i, j) = sum;
		}
	}
	*this = m0;//add here to return the new this
	return  *this;
}

bool Matrix::operator== (const Matrix & m)
{
	if (this->row == m.row && col == m.col)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (std::abs((i, j) - m(i, j)) > 1e-13)
				//if (data_[i * col + j] != m.data_[i * col + j])
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

bool Matrix::operator!= (const Matrix & m)
{
	return !(operator==(m));
}

std::ostream & operator<<(std::ostream & os, const Matrix & m)
{
	int C = m.getColumns();
	int R = m.getRows();
	for (int i = 0; i < R; i++)
	{
		for (int j = 0; j < C; j++)
		{
			os << m(i, j) << " "; //no comma
		}
		os << endl;
	}
	return os;
}

