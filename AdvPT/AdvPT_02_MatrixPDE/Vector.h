#include <iostream>
#include <cassert>
#include <algorithm>
#include <fstream>  
#include <sstream>  
#include <vector>  
#include <string>
#include <array>
#include <cmath>

using namespace std;
using std::vector;
using std::string;
using std::cout;
using std::cin;
using std::istream;
using std::ostream;
using std::endl;

template<class T, size_t LEN>
class Vector
{
public:
	
	Vector<T, LEN>()
	{		
		static_assert(LEN >= 0, "length of vector less than 0");
	}

	Vector<T, LEN>(T initialValue) 
	{
		static_assert(LEN >= 0, "length of vector less than 0");
		
		for (int i = 0; i < LEN; i++)
		{
			data_[i] = initialValue;
		}
	}

	Vector(const Vector & v) 
	{
		std::copy(v.data_.begin(), v.data_.end(), data_.begin());
	}

	~Vector()
	{
		//delete [] data_;
	}

	const T & operator() (int i) const
	{
		assert(i >= 0 && i < LEN);
		return data_[i];
	}

	T & operator() (int i)
	{
		assert(i >= 0 && i < LEN);
		return data_[i];
	}

	Vector<T, LEN> & operator= (const Vector<T, LEN> & v);
	Vector<T, LEN> operator+(const Vector<T, LEN> & v)const;
	Vector<T, LEN> operator-(const Vector<T, LEN> & v)const;

	Vector<T, LEN> & operator+=(const Vector<T, LEN> & v);
	Vector<T, LEN> & operator-=(const Vector<T, LEN> & v);
	bool operator==(const Vector<T, LEN> & v)const;
	bool operator!=(const Vector<T, LEN> & v)const;


	double l2Norm() const
	{
		double norm = 0.;
		for (int i = 0; i < size(); ++i)
		{
			norm += data_[i] * data_[i];
		}
		return sqrt(norm);
	}

	int size() const
	{
		return LEN;
	}

	T GetData(int i) const
	{
		return data_[i];
	}

private:
	//int length;
	std::array<T, LEN> data_;
	//T * data_;

};


template<class T, size_t LEN>
Vector<T, LEN> &Vector<T, LEN>::operator= (const Vector<T, LEN> & v)
{
	for (int i = 0; i < v.LEN; i++)
	{
		data_[i] = v.data_[i];
	}
	return *this;

}


template<class T, size_t LEN>
Vector<T, LEN> Vector<T, LEN>::operator+(const Vector<T, LEN> & v) const
{
	Vector<T, LEN> v0(0);

	for (int i = 0; i < LEN; i++)
	{
		v0(i) = v(i) + data_[i];
	}
	return v0;
}

template<class T, size_t LEN>
Vector<T, LEN> Vector<T, LEN>::operator-(const Vector<T, LEN> & v) const
{
	Vector<T, LEN> v0(0);

	for (int i = 0; i < LEN; i++)
	{
		v0(i) = data_[i] - v(i);
	}
	return v0;
}

template<class T, size_t LEN>
Vector<T, LEN> & Vector<T, LEN>::operator+=(const Vector<T, LEN> & v)
{

	for (int i = 0; i < LEN; i++)
	{
		data_[i] += v(i);
	}
	return *this;
}

template<class T, size_t LEN>
Vector<T, LEN> & Vector<T, LEN>::operator-=(const Vector<T, LEN> & v)
{

	for (int i = 0; i < LEN; i++)
	{
		data_[i] -= v(i);
	}
	return *this;
}

template<class T, size_t LEN>
bool Vector<T, LEN>::operator== (const Vector<T, LEN> & v) const
{
	if (this->LEN == v.LEN)
	{
		for (int i = 0; i < LEN; i++)
		{

			//if (std::abs((i)-v(i)) > 1e-13)//data_[i]
			if (data_[i] != v.data_[i])
			{
				return false;
			}

		}
		return true;

	}
	else
	{
		return false;
	}
}

template<class T, size_t LEN>
bool Vector<T, LEN>::operator!= (const Vector<T, LEN> & v) const
{
	return !(operator==(v));
}

template<class T, size_t LEN>
std::ostream & operator<<(std::ostream & os, const Vector<T, LEN> & v)
{
	for (int i = 0; i < LEN; i++)
	{
		os << v(i) << " "; //no comma
	}
	os << endl;
	return os;
}