#include <iostream>
#include <fstream>  
#include <sstream>   
#include <cassert>
#include <algorithm>
#include <assert.h>


using std::string;
using std::cout;
using std::cin;
using std::istream;
using std::ostream;
using std::endl;


class Matrix {
private:
	int row;
	int col;
	double * data_;

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

	// copy constructor implementation
	Matrix::Matrix(const Matrix& m) {
		//int size = m.rows() * m.cols(); no size here because it is dynamic
		row = m.rows();
		col = m.cols();
		data_ = new double[row*col];
		std::copy(m.data_, m.data_ + row*col, data_);
	}

	~Matrix()
	{
		delete[] data_;
	}

	int cols() const
	{
		return col;
	}

	int rows() const
	{
		return row;
	}

	double & operator() (int i, int j) const//problem here:&
	{
		assert(i >= 0 && i < row && j >= 0 && j < col);
		return data_[i * col + j];
	}

	double & operator() (int i, int j)
	{
		assert(i >= 0 && i < row && j >= 0 && j < col);
		return data_[i * col + j];
	}

	Matrix& operator=(const Matrix&); // copy assignment operator
	bool operator ==(const Matrix&) const; // equality
	bool operator !=(const Matrix&) const;// const to be read only

	// arithmetic operations
	Matrix& operator +=(const Matrix&);
	Matrix operator +(const Matrix&) const;
	Matrix& operator -=(const Matrix&);
	Matrix operator -(const Matrix&) const;
	Matrix& operator *=(const Matrix&);
	Matrix operator *(const Matrix&) const;

};





// copy assignment operator
Matrix& Matrix::operator=(const Matrix& m) //need to check if assignment self
{
	if (&m == this)
		return *this;

	row = m.rows();
	col = m.cols();
	// free current memory and allocate new memory
	Matrix mat_new(row, col, 0.0);
	if (data_ != nullptr) {
		delete[] data_;
		data_ = new double[row*col];
	}

	// element-wise copy the values
	for (int i = 0; i < row * col; ++i)
			data_[i] = m.data_[i];

	return *this;
}



// determine if two matrix are identical
bool Matrix::operator ==(const Matrix& m) const 
{
	if (this->row == m.row && col == m.col)
	{
		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (data_[i * col + j] != m.data_[i * col + j])
					//if (std::abs((i, j) - m(i, j)) > 1e-13)
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

bool Matrix::operator!= (const Matrix & m) const
{
	return !(operator==(m));
}

// !! test unequal size
Matrix& Matrix::operator+=(const Matrix& m) 
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

// construct a new matrix to store the addition of two matrices
Matrix Matrix::operator+(const Matrix& m) const {
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

Matrix& Matrix::operator-=(const Matrix& m) {
	assert(this->row == m.row && col == m.col);

	/*for (int i = 0; i < rows() * cols(); ++i)
			data_[i] -= m.data_[i];*/
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			data_[i*col + j] = data_[i*col + j] - m.data_[i*col + j];//wrong order
		}
	}

	return *this;
}

Matrix Matrix::operator-(const Matrix& m) const {
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

//m*n(i*p) & n*n(p*j)
Matrix & Matrix::operator*= (const Matrix & m)
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
				sum += data_[i * col + p] * m.data_[p * m.col + j];//problem here: m.col
			}
			//data_[i * col + j] = sum;
			m0(i, j) = sum;
		}
	}
	*this = m0;//add here to return the new this
	return *this;
}

Matrix Matrix::operator* (const Matrix & m) const
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



std::ostream & operator<<(std::ostream & os, const Matrix & m)
{
	int C = m.cols();
	int R = m.rows();
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

std::istream & operator >> (std::istream& is, Matrix & m)
{
	for (int i = 0; i < m.rows(); ++i)
	{
		for (int j = 0; j < m.cols(); ++j)
		{
			is >> m(i, j);
		}
	}
	return is;
}




