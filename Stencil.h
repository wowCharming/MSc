#include <cassert>
#include <utility> //std::swap
#include <algorithm> // std::find_if
#include <vector>

#include "MatrixLike.h"

template<typename T>
using StencilEntry = std::pair<int, T>; // convenience type for stencil entries

template<typename T, size_t ROWS, size_t COLS>
class Stencil : public MatrixLike <T, Stencil<T, ROWS, COLS>, ROWS, COLS>/* TODO: inherit MatrixLike */ {
public:
	Stencil(const std::vector<StencilEntry<T> >& boundaryEntries, const std::vector<StencilEntry<T> >& innerEntries)
		: boundaryStencil_(boundaryEntries), innerStencil_(innerEntries) { }
	Stencil(const std::vector<StencilEntry<T> >& innerEntries)	// c'tor for stencils w/o explicit boundary handling
		: boundaryStencil_(innerEntries), innerStencil_(innerEntries) { }

	Stencil<T, ROWS, COLS>(const Stencil & o) : boundaryStencil_(o.boundaryStencil_), innerStencil_(o.innerStencil_)
	{
		boundaryStencil_ = o.boundaryStencil_;
		innerStencil_ = o.innerStencil_;
	};
	//Stencil(Stencil && o) noexcept;

	~Stencil( ) noexcept override { }

	Stencil<T, ROWS, COLS>& operator=(const Stencil<T, ROWS, COLS> & o)
	{
		Stencil<T, ROWS, COLS> tmp(o);
		std::swap(boundaryStencil_, tmp.boundaryStencil_);
		std::swap(innerStencil_, tmp.innerStencil_);
		return *this;
	};

	//Stencil& operator=(Stencil && o) noexcept;

	// HINT: stencil entries are stored as offset/coefficient pair, that is the offset specifies which element of a
	// vector, relative to the current index, is to be regarded. It is then multiplied with the according coefficient.
	// All of these expressions are evaluated and then summed up to get the final result.
	Vector<T,ROWS> operator* (const Vector<T, ROWS> & o) const override
	{
		int len = o.size();
		Vector<T, ROWS> v0(0);
		//cout << len << endl;
		T temp0 = 0;
		for (int k = 0; k < this->boundaryStencil_.size(); k++)
		{
			temp0 += boundaryStencil_[k].second * o(boundaryStencil_[k].first);
			//boundaryStencil_[k].first += o.size();
		}
		v0(0) = temp0;

		T temp1 = 0;
		for (int p = 0; p < this->boundaryStencil_.size(); p++)
		{
			temp1 += boundaryStencil_[p].second * o(boundaryStencil_[p].first + len - 1);
		}
		v0(len - 1) = temp1;//boundaryStencil_[0].second * o(len - 1);

		//innerStencil_ algorithm
		for (int i = 1; i < len - 1; ++i)
		{	
			T sum = 0;
			for (int j = 0; j < innerStencil_.size(); ++j)//innerStencil_.size()
			{
				sum += innerStencil_[j].second * o(innerStencil_[j].first + i);
						//++innerStencil_[j].first;
			}
			v0(i) = sum;
		}
		return v0;
	};

	//determin first
	Stencil<T, ROWS, COLS> inverseDiagonal() const override
	{
		Stencil<T, ROWS, COLS> s0(this->boundaryStencil_, this->innerStencil_);
		for (int k = 0; k < s0.innerStencil_.size(); ++k)
		{
			if (s0.innerStencil_[k].first == 0)
			{
				s0.innerStencil_[k].second = 1 / s0.innerStencil_[k].second;
			}
			else
			{
				s0.innerStencil_[k].second = 0;
			}
		}
		
		s0.boundaryStencil_[0].second = 1 / boundaryStencil_[0].second;

		return s0;
	};
	

protected:
	// containers for the stencil entries -> boundary stencils represent the first and last rows of a corresponding
	// matrix and are to be applied to the first and last element of a target vector; inner stencils correspond to
	// the remaining rows of the matrix
	std::vector<StencilEntry<T> > boundaryStencil_;	// feel free to change the datatype if convenient
	std::vector<StencilEntry<T> > innerStencil_;	// feel free to change the datatype if convenient
};
