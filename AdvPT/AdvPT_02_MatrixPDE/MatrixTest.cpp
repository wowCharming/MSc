#include <iostream>
#include <fstream>  
#include <istream>
#include <string>
#include <cassert>
#include <utility>
#include <algorithm>
#include <functional>
#include <sstream>
#include "Matrix.h"
#include "Vector.h"

using std::size_t;

template <size_t rows, size_t cols>
using MatrixD = Matrix<double, rows, cols>;

class TestCase {
public:
	TestCase(std::string name) :
			name_(name) {
		std::cout << "Test Case START:\t" << name_ << std::endl;
	}
	~TestCase() {
		std::cout << "Test Case END:  \t" << name_ << std::endl;
	}
private:
	std::string name_;
};
#define TESTCASE(name) TestCase _testcase(name)

bool almostEqual(double a, double b, double epsilon = 1e-13) {
	return std::abs(a - b) <= epsilon;
}

// this function allows only matrices of equal dimensions
template <size_t a_rows, size_t a_cols, size_t b_rows, size_t b_cols>
bool almostEqual(const Matrix<double, a_rows, a_cols>& a, const Matrix<double, b_rows, b_cols>& b, double epsilon = 1e-13) {
	static_assert(a_rows == b_rows, "dimension mismatch in comparison");
	static_assert(a_cols == b_cols, "dimension mismatch in comparison");
	for (size_t i = 0; i < a_rows; ++i) {
		for (size_t j = 0; j < a_cols; ++j) {
			if (!almostEqual(a(i, j), b(i, j), epsilon)) {
				return false;
			}
		}
	}
	return true;
}
void test_get_set(double initValue = 1.0) {
	TESTCASE("test_get_set");
    constexpr size_t rows = 2;
    constexpr size_t cols = 4;
    using MatrixDRC = MatrixD<rows, cols>;
	const MatrixDRC m1(initValue);
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			assert("const get check" && m1(i, j) == initValue);
		}
	}
	// compile time check: taking address of const reference
	assert(*&m1(0, 0) == m1(0, 0));

	MatrixDRC m2(0.0);
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			m2(i, j) = initValue;
			assert("set check" && m2(i, j) == initValue);
		}
	}
	auto begin = &m2(0, 0);
	auto end = &m2(rows - 1, cols - 1) + 1;
	size_t diff = end - begin;
	assert("1D memory layout check" && (diff == rows * cols));
	assert("row wise order check" && (&m2(0, 1) - &m2(0, 0) == 1));
	assert("row wise order check" && (size_t(&m2(1, 0) - &m2(0, 0)) == cols));
}

void test_memory(double initValue = 1.0) {
	TESTCASE("test_memory");
    constexpr size_t rows = 2;
    constexpr size_t cols = 4;
    using MatrixDRC = MatrixD<rows, cols>;
	const MatrixDRC m1(initValue);
    const MatrixDRC copy(m1);
	assert("copy ctr check" && almostEqual(m1, copy, 0));
	assert("no alias check" && &copy(0, 0) != &m1(0, 0));
	MatrixDRC m2(initValue);
	m2 = m1;
	assert("assignment check" && almostEqual(m1, m2, 0));
	assert("no alias check" && &m1(0, 0) != &m2(0, 0));
	// allow zero dimensions
	Matrix<double, 0, 0> m3(0.0);
	auto initValue2 = 1.5;
	MatrixDRC m4(initValue2);
	MatrixDRC m4copy(m4);
	m4 = m4;
	// check self assignment
	assert("self assignment check" && almostEqual(m4, m4copy, 0));
}

void test_compare(double v = 1.0) {
	TESTCASE("test_compare");
    constexpr size_t rows = 2;
    constexpr size_t cols = 4;
    using MatrixDRC = MatrixD<rows, cols>;
    using MatrixDCR = MatrixD<cols, rows>;
    using MatrixD0 = MatrixD<0, 0>;
    {
	    MatrixDRC m1(v);
	    assert("compare check" && m1 == m1);
	    assert("compare check" && !(m1 != m1));
	    assert("compare check" && m1 == MatrixDRC(m1));//check MatrixDRC(m1)
	    assert("compare check" && !(m1 != MatrixDRC(m1)));
	    const MatrixDRC m2 = m1;
	    m1(rows - 1, cols - 1) += 1;
	    assert("compare check" && m1 != m2);
	    assert("compare check" && !(m1 == m2));
    }
	assert("compare check" && MatrixDRC(v) == MatrixDRC(v));
	assert("compare check" && !(MatrixDRC(v) != MatrixDRC(v)));
	// allow comparison of matrices of different dimensions
	assert("compare check" && MatrixDCR(v) != MatrixDRC(v));
	assert("compare check" && !(MatrixDCR(v) == MatrixDRC(v)));
	assert("compare check" && MatrixD0(0) != MatrixDRC(v));
	assert("compare check" && !(MatrixD0(0) == MatrixDRC(v)));
	assert("compare check" && MatrixD0(0) == MatrixD0(0));
	assert("compare check" && !(MatrixD0(0) != MatrixD0(0)));
}

void test_arithmetic() {
	TESTCASE("test_arithmetic");
    constexpr size_t rows = 2;
    constexpr size_t cols = 4;
    using MatrixDRC = MatrixD<rows, cols>;
	std::function<double(size_t, size_t)> f =
			[](size_t i, size_t j) {return j * 100 + i;};
	MatrixDRC m1(0);
	MatrixDRC m2(0);
	MatrixDRC sum_result(0);
	MatrixDRC diff_result(0);
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			m1(i, j) = f(i, j);
			m2(i, j) = 2 * f(i, j);
            sum_result(i, j) = m1(i, j) + m2(i, j);
            diff_result(i, j) = m1(i, j) - m2(i, j);
		}
	}
	MatrixDRC sum1 = m1 + m2;
	MatrixDRC sum2 = m1;
	sum2 += m2;
	MatrixDRC diff1 = m1 - m2;
	MatrixDRC diff2 = m1;
	diff2 -= m2;

    assert("check operator+" && almostEqual(sum_result, sum1));
    assert("check operator+=" && almostEqual(sum_result, sum2));
    assert("check operator-" && almostEqual(diff_result, diff1));
    assert("check operator-=" && almostEqual(diff_result, diff2));

	double a = 3, b = 4;
	MatrixD<1, 1> prod11 = MatrixD<1, 1>(a) * MatrixD<1, 1>(b);
	assert("check operator*" && almostEqual(prod11, MatrixD<1, 1>(a * b)));
	constexpr size_t n1 = 10;
	prod11 = MatrixD<1, n1>(a) * MatrixD<n1, 1>(b);
	assert("check operator*" && almostEqual(prod11, MatrixD<1, 1>(n1 * a * b)));
	auto prod23 = MatrixD<2, n1>(a) * MatrixD<n1, 3>(b);
	assert("check operator*" && almostEqual(prod23, MatrixD<2, 3>(n1 * a * b)));
	auto prodn1n1 = MatrixD<n1, 1>(a) * MatrixD<1, n1>(b);
	assert("check operator*" && almostEqual(prodn1n1, MatrixD<n1, n1>(a * b)));
	constexpr size_t n2 = 63000;//100000;
	prod11 = MatrixD<1, n2>(a) * MatrixD<n2, 1>(b);
	assert("check operator*" && almostEqual(prod11, MatrixD<1, 1>(n2 * a * b)));
	prodn1n1 = MatrixD<n1, n1>(a);
	prodn1n1 *= MatrixD<n1, n1>(b);
	assert("check operator*=" && almostEqual(prodn1n1, MatrixD<n1, n1>(n1 * a * b)));
    MatrixD<n1, n1> unity(0);
    Vector<double, n1> v(0);
    for (size_t i = 0; i < n1; ++i) {
        unity(i, i) = 1;
        v(i) = f(i, i);
    }
    auto result = unity * v;
    for (size_t i = 0; i < n1; ++i) {
    	assert("check matrix * vector" && almostEqual(v(i), result(i)));
    }
}

void test_input_output_self_consistency() {
	TESTCASE("test_input_output_self_consistency");
    constexpr size_t rows = 2;
    constexpr size_t cols = 4;
	MatrixD<rows, cols> m1(0);
	MatrixD<rows, cols> m2(0);
	for (size_t i = 0; i < rows; ++i) {
		for (size_t j = 0; j < cols; ++j) {
			m1(i, j) = (j * 100 + i) / 9.0;
		}
	}
	std::stringstream ss;
	ss << m1;
	ss >> m2;
	// checking if your operator>> can parse your own output generated by operator<<
	assert("check output and input operator" && almostEqual(m1, m2, 1e-4));
}

int main() {
	test_get_set();
	test_memory();
	test_compare();
	test_arithmetic();
	test_input_output_self_consistency();
    std::cout << "all tests finished without assertion errors" << std::endl;
	getchar();
}


// static compile-time checks whether data is placed on the stack
// n1 may be greater than n2, since underflow/overflow is well defined for unsigned integer arithmetic (wrap around)
template <class T, size_t n1,  size_t n2>
void static_stack_usage_check_matrix() {
    constexpr size_t diff = sizeof(Matrix<T, n2, n2>) - sizeof(Matrix<T, n1, n1>);
    constexpr size_t diff_reference = (n2 * n2 - n1 * n1) * sizeof(T);
    static_assert(diff == diff_reference, "Matrix class must store its data on the stack");
}

template <class T, size_t n1,  size_t n2>
void static_stack_usage_check_vector() {
    constexpr size_t diff = sizeof(Vector<T, n2>) - sizeof(Vector<T, n1>);
    constexpr size_t diff_reference = (n2 - n1) * sizeof(T);
    static_assert(diff == diff_reference, "Vector class must store its data on the stack");
}

// explicit template function instantiations
template void static_stack_usage_check_matrix<double, 123, 456>();
template void static_stack_usage_check_vector<double, 321, 123>();
