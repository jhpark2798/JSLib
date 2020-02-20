#pragma once
#include "../Disposable.h"
#include <boost/iterator/reverse_iterator.hpp>
#include <boost/scoped_array.hpp>
#include "../Null.h"
#include <iostream>
#include "functional.h"
#include <functional>
#include <numeric>
#include <stdexcept>
#include <iomanip>

namespace JSLib {

	class Array {
	public:
		explicit Array(size_t size = 0);
		Array(size_t size, double value);
		Array(size_t size, double value, double increment);
		Array(const Array&);
		Array(const Disposable<Array>&);

		template<class ForwardIterator>
		Array(ForwardIterator begin, ForwardIterator end);

		Array& operator=(const Array&);
		Array& operator=(const Disposable<Array>&);
		bool operator==(const Array&) const;
		bool operator!=(const Array&) const;

		const Array& operator+=(const Array&);
		const Array& operator+=(double);
		const Array& operator-=(const Array&);
		const Array& operator-=(double);
		const Array& operator*=(const Array&);
		const Array& operator*=(double);
		const Array& operator/=(const Array&);
		const Array& operator/=(double);

		double operator[](size_t) const;
		double at(size_t) const;
		double front() const;
		double back() const;

		double& operator[](size_t);
		double& at(size_t);
		double& front();
		double& back();

		size_t size() const;
		bool empty() const;
		
		typedef size_t size_type;
		typedef double value_type;
		typedef double* iterator;
		typedef const double* const_iterator;
		typedef boost::reverse_iterator<iterator> reverse_iterator;
		typedef const boost::reverse_iterator<const_iterator> const_reverse_iterator;

		const_iterator begin() const;
		iterator begin();
		const_iterator end() const;
		iterator end();
		const_reverse_iterator rbegin() const;
		reverse_iterator rbegin();
		const_reverse_iterator rend() const;
		reverse_iterator rend();

		void resize(size_t n);
		void swap(Array&);

	private:
		boost::scoped_array<double> data_; //scoped_array인 이유?
		size_t n_;
	};

	template <>
	class Null<Array> {
	public:
		Null() {}
		operator Array() const { return Array(); }
	};

	double DotProduct(const Array&, const Array&);
	double Norm2(const Array&);

	const Disposable<Array> operator+(const Array& v); //Array 앞에 +, -를 그냥 붙이는 것
	const Disposable<Array> operator-(const Array& v);

	const Disposable<Array> operator+(const Array&, const Array&);
	const Disposable<Array> operator+(const Array&, double);
	const Disposable<Array> operator+(double, const Array&);
	const Disposable<Array> operator-(const Array&, const Array&);
	const Disposable<Array> operator-(const Array&, double);
	const Disposable<Array> operator-(double, const Array&);
	const Disposable<Array> operator*(const Array&, const Array&);
	const Disposable<Array> operator*(const Array&, double);
	const Disposable<Array> operator*(double, const Array&);
	const Disposable<Array> operator/(const Array&, const Array&);
	const Disposable<Array> operator/(const Array&, double);
	const Disposable<Array> operator/(double, const Array&);

	const Disposable<Array> Abs(const Array&);
	const Disposable<Array> Sqrt(const Array&);
	const Disposable<Array> Log(const Array&);
	const Disposable<Array> Exp(const Array&);
	const Disposable<Array> Pow(const Array&, double);

	void swap(Array&, Array&);

	std::ostream& operator<<(std::ostream&, const Array&);

	inline Array::Array(size_t size) : data_(size ? new double[size] : (double*)(0)), n_(size) {}

	inline Array::Array(size_t size, double value) : data_(size ? new double[size] : (double*)(0)), n_(size) { std::fill(begin(), end(), value); }

	inline Array::Array(size_t size, double value, double increment) : data_(size ? new double[size] : (double*)(0)), n_(size)
	{
		for (iterator i = begin(); i != end(); ++i, value += increment)
			*i = value;
	}

	inline Array::Array(const Array& from) : data_(from.n_ ? new double[from.n_] : (double*)(0)), n_(from.n_)
	{
		std::copy(from.begin(), from.end(), begin());
	}

	inline Array::Array(const Disposable<Array>& from) : data_((double*)(0)), n_(0) { swap(const_cast<Disposable<Array>&>(from)); }

	namespace detail {

		template <class I> //begin: size, end: value
		inline void _fill_array(Array& a, boost::scoped_array<double>& data_, size_t& n_, I begin, I end, const boost::true_type&)
		{
			size_t n = begin;
			double value = end;
			data_.reset(n ? new double[n] : (double*)(0));
			n_ = n;
			std::fill(a.begin(), a.end(), value);
		}

		template <class I> //begin, end: iterator
		inline void _fill_array(Array& a, boost::scoped_array<double>& data_, size_t& n_, I begin, I end, const boost::false_type&)
		{
			size_t n = std::distance(begin, end);
			data_.reset(n ? new double[n] : (double*)(0));
			n_ = n;
			std::copy(begin, end, a.begin());
		}

	}
	template <class ForwardIterator>
	inline Array::Array(ForwardIterator begin, ForwardIterator end)
	{
		detail::_fill_array(*this, data_, n_, begin, end, boost::is_integral<ForwardIterator>());
	}

	inline Array& Array::operator=(const Array& from)
	{
		Array temp(from);
		swap(temp);
		return *this;
	}

	inline bool Array::operator==(const Array& to) const
	{
		return (n_ == to.n_) && std::equal(begin(), end(), to.begin());
	}

	inline bool Array::operator!=(const Array& to) const
	{
		return !(this->operator==(to));
	}

	inline Array& Array::operator=(const Disposable<Array>& from)
	{
		swap(const_cast<Disposable<Array>&>(from));
		return *this;
	}

	inline const Array& Array::operator+=(const Array& v)
	{
		std::transform(begin(), end(), v.begin(), begin(), std::plus<double>());
		return *this;
	}

	inline const Array& Array::operator+=(double x)
	{
		std::transform(begin(), end(), begin(), add<double>(x));
		return *this;
	}

	inline const Array& Array::operator-=(const Array& v)
	{
		std::transform(begin(), end(), v.begin(), begin(), std::minus<double>());
		return *this;
	}

	inline const Array& Array::operator-=(double x)
	{
		std::transform(begin(), end(), begin(), subtract<double>(x));
		return *this;
	}

	inline const Array& Array::operator*=(const Array& v)
	{
		std::transform(begin(), end(), v.begin(), begin(), std::multiplies<double>());
		return *this;
	}

	inline const Array& Array::operator*=(double x)
	{
		std::transform(begin(), end(), begin(), multiply_by<double>(x));
		return *this;
	}

	inline const Array& Array::operator/=(const Array& v)
	{
		std::transform(begin(), end(), v.begin(), begin(), std::divides<double>());
		return *this;
	}

	inline const Array& Array::operator/=(double x)
	{
		std::transform(begin(), end(), begin(), divide_by<double>(x));
		return *this;
	}

	inline double Array::operator[](size_t i) const
	{
		if (i >= n_)
			throw std::domain_error("array access out of range.");
		else
			return data_.get()[i];
	}

	inline double Array::at(size_t i) const
	{
		if (i >= n_)
			throw std::domain_error("array access out of range.");
		else
			return data_.get()[i];
	}

	inline double Array::front() const
	{
		return data_.get()[0];
	}

	inline double Array::back() const
	{
		return data_.get()[n_ - 1];
	}

	inline double& Array::operator[](size_t i)
	{
		if (i >= n_)
			throw std::domain_error("array access out of range.");
		else
			return data_.get()[i];
	}

	inline double& Array::at(size_t i)
	{
		if (i >= n_)
			throw std::domain_error("array access out of range.");
		else
			return data_.get()[i];
	}

	inline double& Array::front()
	{
		return data_.get()[0];
	}

	inline double& Array::back()
	{
		return data_.get()[n_ - 1];
	}

	inline size_t Array::size() const
	{
		return n_;
	}

	inline bool Array::empty() const
	{
		return n_ == 0;
	}

	inline Array::const_iterator Array::begin() const
	{
		return data_.get();
	}

	inline Array::iterator Array::begin()
	{
		return data_.get();
	}

	inline Array::const_iterator Array::end() const
	{
		return data_.get() + n_;
	}

	inline Array::iterator Array::end()
	{
		return data_.get() + n_;
	}

	inline Array::const_reverse_iterator Array::rbegin() const
	{
		return const_reverse_iterator(end());
	}

	inline Array::reverse_iterator Array::rbegin()
	{
		return reverse_iterator(end());
	}

	inline Array::const_reverse_iterator Array::rend() const
	{
		return const_reverse_iterator(begin());
	}

	inline Array::reverse_iterator Array::rend()
	{
		return reverse_iterator(begin());
	}

	inline void Array::resize(size_t n)
	{
		if (n_ > n) {
			Array swp(n);
			std::copy(begin(), end(), swp.begin());
			swap(swp);
		}
		else if (n < n_) {
			n_ = n;
		}
	}

	inline void Array::swap(Array& from)
	{
		using std::swap;
		data_.swap(from.data_);
		swap(n_, from.n_);
	}

	inline double DotProduct(const Array& v1, const Array& v2)
	{
		return std::inner_product(v1.begin(), v1.end(), v2.begin(), 0.0);
	}

	inline double Norm2(const Array& v)
	{
		return std::sqrt(DotProduct(v, v));
	}

	inline const Disposable<Array> operator+(const Array& v)
	{
		Array result = v;
		return result;
	}

	inline const Disposable<Array> operator-(const Array& v)
	{
		Array result(v.size());
		std::transform(v.begin(), v.end(), result.begin(), std::negate<double>());
		return result;
	}

	inline const Disposable<Array> operator+(const Array& v1, const Array& v2)
	{
		Array result(v1.size());
		std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(), std::plus<double>());
		return result;
	}

	inline const Disposable<Array> operator+(const Array& v1, double a)
	{
		Array result(v1.size());
		std::transform(v1.begin(), v1.end(), result.begin(), add<double>(a));
		return result;
	}

	inline const Disposable<Array> operator+(double a, const Array& v2)
	{
		Array result(v2.size());
		std::transform(v2.begin(), v2.end(), result.begin(), add<double>(a));
		return result;
	}

	inline const Disposable<Array> operator-(const Array& v1, const Array& v2)
	{
		Array result(v1.size());
		std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(), std::minus<double>());
		return result;
	}

	inline const Disposable<Array> operator-(const Array& v1, double a)
	{
		Array result(v1.size());
		std::transform(v1.begin(), v1.end(), result.begin(), subtract<double>(a));
		return result;
	}

	inline const Disposable<Array> operator-(double a, const Array& v2)
	{
		Array result(v2.size());
		std::transform(v2.begin(), v2.end(), result.begin(), subtract_from<double>(a));
		return result;
	}

	inline const Disposable<Array> operator*(const Array& v1, const Array& v2)
	{
		Array result(v1.size());
		std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(), std::multiplies<double>());
		return result;
	}

	inline const Disposable<Array> operator*(const Array& v1, double a)
	{
		Array result(v1.size());
		std::transform(v1.begin(), v1.end(), result.begin(), multiply_by<double>(a));
		return result;
	}

	inline const Disposable<Array> operator*(double a, const Array& v2)
	{
		Array result(v2.size());
		std::transform(v2.begin(), v2.end(), result.begin(), multiply_by<double>(a));
		return result;
	}

	inline const Disposable<Array> operator/(const Array& v1, const Array& v2)
	{
		Array result(v1.size());
		std::transform(v1.begin(), v1.end(), v2.begin(), result.begin(), std::divides<double>());
		return result;
	}

	inline const Disposable<Array> operator/(const Array& v1, double a)
	{
		Array result(v1.size());
		std::transform(v1.begin(), v1.end(), result.begin(), divide_by<double>(a));
		return result;
	}

	inline const Disposable<Array> operator/(double a, const Array& v2)
	{
		Array result(v2.size());
		std::transform(v2.begin(), v2.end(), result.begin(), divide_by<double>(a));
		return result;
	}

	inline const Disposable<Array> Abs(const Array& v)
	{
		Array result(v.size());
		std::transform(v.begin(), v.end(), result.begin(), static_cast<double(*)(double)>(std::fabs)); 
		//double(*)(double) -> double형 변수를 받아서 double형 타입을 반환하는 함수 포인터
		return result;
	}

	inline const Disposable<Array> Sqrt(const Array& v)
	{
		Array result(v.size());
		std::transform(v.begin(), v.end(), result.begin(), static_cast<double(*)(double)>(std::sqrt));
		return result;
	}

	inline const Disposable<Array> Log(const Array& v)
	{
		Array result(v.size());
		std::transform(v.begin(), v.end(), result.begin(), static_cast<double(*)(double)>(std::log));
		return result;
	}

	inline const Disposable<Array> Exp(const Array& v)
	{
		Array result(v.size());
		std::transform(v.begin(), v.end(), result.begin(), static_cast<double(*)(double)>(std::exp));
		return result;
	}

	inline const Disposable<Array> Pow(const Array& v, double alpha)
	{
		Array result(v.size());
		for (size_t i = 0; i < v.size(); ++i)
			result[i] = std::pow(v[i], alpha);
		return result;
	}

	inline void swap(Array& v, Array& w)
	{
		v.swap(w);
	}

	inline std::ostream& operator<<(std::ostream& out, const Array& a)
	{
		std::streamsize width = out.width();
		out << "[ ";
		if (!a.empty()) {
			for (size_t n = 0; n < a.size()-1; ++n) 
				out << std::setw(int(width)) << a[n] << "; ";
			out << std::setw(int(width)) << a.back();
		}
		out << " ]";
		return out;
	}

}

