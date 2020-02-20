#pragma once
#include "Array.h" 
#include "../steppingiterator.h"
#include <algorithm>

namespace JSLib {

	class Matrix {
	public:
		Matrix();
		Matrix(size_t rows, size_t columns);
		Matrix(size_t rows, size_t columns, double value);

		template <class Iterator>
		Matrix(size_t rows, size_t columns, Iterator begin, Iterator end);

		Matrix(const Matrix&);
		Matrix(const Disposable<Matrix>&);
		Matrix& operator=(const Matrix&);
		Matrix& operator=(const Disposable<Matrix>&);

		const Matrix& operator+=(const Matrix&);
		const Matrix& operator-=(const Matrix&);
		const Matrix& operator*=(double);
		const Matrix& operator/=(double);

		typedef double* iterator;
		typedef const double* const_iterator;
		typedef boost::reverse_iterator<iterator> reverse_iterator;
		typedef boost::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef double* row_iterator;
		typedef const double* const_row_iterator;
		typedef boost::reverse_iterator<row_iterator> reverse_row_iterator;
		typedef boost::reverse_iterator<const_row_iterator> const_reverse_row_iterator;
		typedef step_iterator<iterator> column_iterator;
		typedef step_iterator<const_iterator> const_column_iterator;
		typedef boost::reverse_iterator<column_iterator> reverse_column_iterator;
		typedef boost::reverse_iterator<const_column_iterator> const_reverse_column_iterator;

		const_iterator begin() const;
		iterator begin();
		const_iterator end() const;
		iterator end();
		const_reverse_iterator rbegin() const;
		reverse_iterator rbegin();
		const_reverse_iterator rend() const;
		reverse_iterator rend();
		const_row_iterator row_begin(size_t i) const;
		row_iterator row_begin(size_t i);
		const_row_iterator row_end(size_t i) const;
		row_iterator row_end(size_t i);
		const_reverse_row_iterator row_rbegin(size_t i) const;
		reverse_row_iterator row_rbegin(size_t i);
		const_reverse_row_iterator row_rend(size_t i) const;
		reverse_row_iterator row_rend(size_t i);
		const_column_iterator column_begin(size_t i) const;
		column_iterator column_begin(size_t i);
		const_column_iterator column_end(size_t i) const;
		column_iterator column_end(size_t i);
		const_reverse_column_iterator column_rbegin(size_t i) const;
		reverse_column_iterator column_rbegin(size_t i);
		const_reverse_column_iterator column_rend(size_t i) const;
		reverse_column_iterator column_rend(size_t i);

		const_row_iterator operator[](size_t) const;
		const_row_iterator at(size_t) const;
		row_iterator operator[](size_t);
		row_iterator at(size_t);
		Disposable<Array> diagonal() const;
		double& operator()(size_t i, size_t j) const;

		size_t rows() const;
		size_t columns() const;
		bool empty() const;
		size_t size1() const;
		size_t size2() const;

		void swap(Matrix&);

	private:
		boost::scoped_array<double> data_;
		size_t rows_, columns_;
	};

	const Disposable<Matrix> operator+(const Matrix&, const Matrix&);
	const Disposable<Matrix> operator-(const Matrix&, const Matrix&);
	const Disposable<Matrix> operator*(const Matrix&, double);
	const Disposable<Matrix> operator*(double, const Matrix&);
	const Disposable<Matrix> operator/(const Matrix&, double);

	const Disposable<Array> operator*(const Array&, const Matrix&);
	const Disposable<Array> operator*(const Matrix&, const Array&);
	const Disposable<Matrix> operator*(const Matrix&, const Matrix&);

	const Disposable<Matrix> transpose(const Matrix&);
	const Disposable<Matrix> outerProduct(const Array&, const Array&);

	template <class Iterator1, class Iterator2>
	const Disposable<Matrix> outerProduct(Iterator1 v1begin, Iterator1 v1end, Iterator2 v2begin, Iterator2 v2end);

	void swap(Matrix&, Matrix&);

	std::ostream& operator<<(std::ostream&, const Matrix&);

	Disposable<Matrix> inverse(const Matrix&);

	double determinant(const Matrix&);

	inline Matrix::Matrix() : data_((double*)(0)), rows_(0), columns_(0) {}

	inline Matrix::Matrix(size_t rows, size_t columns) : data_(rows* columns > 0 ? new double[rows * columns] : (double*)(0)), rows_(rows), columns_(columns) {}

	inline Matrix::Matrix(size_t rows, size_t columns, double value) : data_(rows* columns > 0 ? new double[rows * columns] : (double*)(0)), rows_(rows), columns_(columns)
	{
		std::fill(begin(), end(), value);
	}

	template <class Iterator>
	inline Matrix::Matrix(size_t rows, size_t columns, Iterator begin, Iterator end)
		:data_(rows* columns > 0 ? new double[rows * columns] : (double*)(0)), rows_(rows), columns_(columns)
	{
		std::copy(begin, end, this->begin());
	}

	inline Matrix::Matrix(const Matrix& from)
		: data_(!from.empty() ? new double[from.rows_ * from.columns_] : (double*)(0)), rows_(from.rows_), columns_(from.columns_)
	{
		std::copy(from.begin(), from.end(), begin());
	}

	inline Matrix::Matrix(const Disposable<Matrix>& from) : data_((double*)(0)), rows_(0), columns_(0) { swap(const_cast<Disposable<Matrix>&>(from)); }

	inline Matrix& Matrix::operator=(const Matrix& from)
	{
		Matrix temp(from);
		swap(temp);
		return *this;
	}

	inline Matrix& Matrix::operator=(const Disposable<Matrix>& from)
	{
		swap(const_cast<Disposable<Matrix>&>(from));
		return *this;
	}

	inline void Matrix::swap(Matrix& from)
	{
		using std::swap;
		data_.swap(from.data_);
		swap(rows_, from.rows_);
		swap(columns_, from.columns_);
	}

	inline const Matrix& Matrix::operator+=(const Matrix& m)
	{
		std::transform(begin(), end(), m.begin(), begin(), std::plus<double>());
		return *this;
	}

	inline const Matrix& Matrix::operator-=(const Matrix& m)
	{
		std::transform(begin(), end(), m.begin(), begin(), std::minus<double>());
		return *this;
	}

	inline const Matrix& Matrix::operator*=(double x)
	{
		std::transform(begin(), end(), begin(), multiply_by<double>(x));
		return *this;
	}

	inline const Matrix& Matrix::operator/=(double x)
	{
		std::transform(begin(), end(), begin(), divide_by<double>(x));
		return *this;
	}

	inline Matrix::const_iterator Matrix::begin() const
	{
		return data_.get();
	}

	inline Matrix::iterator Matrix::begin()
	{
		return data_.get();
	}

	inline Matrix::const_iterator Matrix::end() const
	{
		return data_.get() + rows_ * columns_;
	}

	inline Matrix::iterator Matrix::end()
	{
		return data_.get() + rows_ * columns_;
	}

	inline Matrix::const_reverse_iterator Matrix::rbegin() const
	{
		return const_reverse_iterator(end());
	}

	inline Matrix::reverse_iterator Matrix::rbegin()
	{
		return reverse_iterator(end());
	}

	inline Matrix::const_reverse_iterator Matrix::rend() const
	{
		return const_reverse_iterator(begin());
	}

	inline Matrix::reverse_iterator Matrix::rend()
	{
		return reverse_iterator(begin());
	}

	inline Matrix::const_row_iterator Matrix::row_begin(size_t i) const
	{
		return data_.get() + columns_ * i;
	}

	inline Matrix::row_iterator Matrix::row_begin(size_t i)
	{
		return data_.get() + columns_ * i;
	}

	inline Matrix::const_row_iterator Matrix::row_end(size_t i) const
	{
		return data_.get() + columns_ * (i + 1);
	}

	inline Matrix::row_iterator Matrix::row_end(size_t i)
	{
		return data_.get() + columns_ * (i + 1);
	}

	inline Matrix::const_reverse_row_iterator Matrix::row_rbegin(size_t i) const
	{
		return const_reverse_row_iterator(row_end(i));
	}

	inline Matrix::reverse_row_iterator Matrix::row_rbegin(size_t i)
	{
		return reverse_row_iterator(row_end(i));
	}

	inline Matrix::const_reverse_row_iterator Matrix::row_rend(size_t i) const
	{
		return const_reverse_row_iterator(row_begin(i));
	}

	inline Matrix::reverse_row_iterator Matrix::row_rend(size_t i)
	{
		return reverse_row_iterator(row_begin(i));
	}

	inline Matrix::const_column_iterator Matrix::column_begin(size_t i) const
	{
		return const_column_iterator(data_.get() + i, columns_);
	}

	inline Matrix::column_iterator Matrix::column_begin(size_t i)
	{
		return column_iterator(data_.get() + i, columns_);
	}

	inline Matrix::const_column_iterator Matrix::column_end(size_t i) const
	{
		return const_column_iterator(data_.get() + i + rows_ * columns_, columns_);
	}

	inline Matrix::column_iterator Matrix::column_end(size_t i)
	{
		return column_iterator(data_.get() + i + rows_ * columns_, columns_);
	}

	inline Matrix::const_reverse_column_iterator Matrix::column_rbegin(size_t i) const
	{
		return const_reverse_column_iterator(column_end(i));
	}

	inline Matrix::reverse_column_iterator Matrix::column_rbegin(size_t i)
	{
		return reverse_column_iterator(column_end(i));
	}

	inline Matrix::const_reverse_column_iterator Matrix::column_rend(size_t i) const
	{
		return const_reverse_column_iterator(column_begin(i));
	}

	inline Matrix::reverse_column_iterator Matrix::column_rend(size_t i)
	{
		return reverse_column_iterator(column_begin(i));
	}

	inline Matrix::const_row_iterator Matrix::operator[](size_t i) const
	{
		return row_begin(i);
	}

	inline Matrix::const_row_iterator Matrix::at(size_t i) const
	{
		return row_begin(i);
	}

	inline Matrix::row_iterator Matrix::operator[](size_t i)
	{
		return row_begin(i);
	}

	inline Matrix::row_iterator Matrix::at(size_t i)
	{
		return row_begin(i);
	}

	inline Disposable<Array> Matrix::diagonal() const
	{
		size_t arraySize = std::min<size_t>(rows(), columns());
		Array tmp(arraySize);
		for (size_t i = 0; i < arraySize; ++i) {
			tmp[i] = (*this)[i][i];
		}
		return tmp;
	}

	inline double& Matrix::operator()(size_t i, size_t j) const
	{
		return data_[i * columns() + j];
	}

	inline size_t Matrix::rows() const
	{
		return rows_;
	}

	inline size_t Matrix::columns() const
	{
		return columns_;
	}

	inline size_t Matrix::size1() const
	{
		return rows();
	}

	inline size_t Matrix::size2() const
	{
		return columns();
	}

	inline bool Matrix::empty() const
	{
		return rows_ == 0 || columns_ == 0;
	}

	inline const Disposable<Matrix> operator+(const Matrix& m1, const Matrix& m2)
	{
		Matrix temp(m1.rows(), m1.columns());
		std::transform(m1.begin(), m1.end(), m2.begin(), temp.begin(), std::plus<double>());
		return temp;
	}

	inline const Disposable<Matrix> operator-(const Matrix& m1, const Matrix& m2)
	{
		Matrix temp(m1.rows(), m1.columns());
		std::transform(m1.begin(), m1.end(), m2.begin(), temp.begin(), std::minus<double>());
		return temp;
	}

	inline const Disposable<Matrix> operator*(const Matrix& m, double x)
	{
		Matrix temp(m.rows(), m.columns());
		std::transform(m.begin(), m.end(), temp.begin(), multiply_by<double>(x));
		return temp;
	}

	inline const Disposable<Matrix> operator*(double x, const Matrix& m)
	{
		Matrix temp(m.rows(), m.columns());
		std::transform(m.begin(), m.end(), temp.begin(), multiply_by<double>(x));
		return temp;
	}

	inline const Disposable<Matrix> operator/(const Matrix& m, double x)
	{
		Matrix temp(m.rows(), m.columns());
		std::transform(m.begin(), m.end(), temp.begin(), divide_by<double>(x));
		return temp;
	}

	inline const Disposable<Array> operator*(const Array& v, const Matrix& m)
	{
		Array result(m.columns());
		for (size_t i = 0; i < result.size(); ++i) {
			result[i] = std::inner_product(v.begin(), v.end(), m.column_begin(i), 0.0);
		}
		return result;
	}

	inline const Disposable<Array> operator*(const Matrix& m, const Array& v)
	{
		Array result(m.rows());
		for (size_t i = 0; i < result.size(); ++i) {
			result[i] = std::inner_product(v.begin(), v.end(), m.row_begin(i), 0.0);
		}
		return result;
	}

	inline const Disposable<Matrix> operator*(const Matrix& m1, const Matrix& m2)
	{
		Matrix result(m1.rows(), m2.columns(),0.0);
		for (size_t i = 0; i < result.rows(); ++i) {
			for (size_t j = 0; j < result.columns(); ++j) {
				for (size_t k = 0; k < m1.columns(); ++k) {
					result[i][j] = result[i][j] + m1[i][k] * m2[k][j];
				}
			}
		}
		return result;
	}

	inline const Disposable<Matrix> transpose(const Matrix& m)
	{
		Matrix result(m.columns(), m.rows());
		for (size_t i = 0; i < m.columns(); ++i) {
			std::copy(m.row_begin(i), m.row_end(i), result.column_begin(i));
		}
		return result;
	}

	inline const Disposable<Matrix> outerProduct(const Array& v1, const Array& v2)
	{
		return outerProduct(v1.begin(), v1.end(), v2.begin(), v2.end());
	}

	template <class Iterator1, class Iterator2>
	inline const Disposable<Matrix> outerProduct(Iterator1 v1begin, Iterator1 v1end, Iterator2 v2begin, Iterator2 v2end)
	{
		size_t size1 = std::distance(v1begin, v1end);
		size_t size2 = std::distance(v2begin, v2end);

		Matrix result(size1, size2);

		for (size_t i = 0; v1begin != v1end; ++i, ++v1begin) {
			std::transform(v2begin, v2end, result.row_begin(i), multiply_by<double>(*v1begin));
		}
		return result;
	}

	inline void swap(Matrix& m1, Matrix& m2)
	{
		m1.swap(m2);
	}

	inline std::ostream& operator<<(std::ostream& out, const Matrix& m)
	{
		std::streamsize width = out.width();
		for (size_t i = 0; i < m.rows(); ++i) {
			out << "| ";
			for (size_t j = 0; j < m.columns(); ++j) {
				out << std::setw(int(width)) << m[i][j] << " ";
			}
			out << "|\n";
		}
		return out;
	}

}