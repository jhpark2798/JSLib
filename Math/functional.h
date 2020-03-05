#pragma once
#include "..//Null.h"
#include <cmath>

//클래스의 멤버변수가 연산에 사용되는 단위
//ex) A=divide(3)으로 클래스 생성하면 A(x)로 x/3을 구할 수 있음

namespace JSLib {

	template <class T, class U>
	class constant {
	public:
		typedef T argument_type;
		typedef U result_type;
		explicit constant(const U& u) : u_(u) {}
		U operator()(const T&) const { return u_; }

	private:
		U u_;
	};

	template <class T>
	class identity {
	public:
		typedef T argument_type;
		typedef T result_type;
		T operator()(const T& t) const { return t; }
	};

	template <class T>
	class square {
	public:
		typedef T argument_type;
		typedef T result_type;
		T operator()(const T& t) const { return t * t; }
	};

	template <class T>
	class cube {
	public:
		typedef T argument_type;
		typedef T result_type;
		T operator()(const T& t) const { return t * t * t; }
	};

	template <class T>
	class fourth_power {
	public:
		typedef T argument_type;
		typedef T result_type;
		T operator()(const T& t) const { return t * t * t * t; }
	};

	template <class T>
	class add {
		T y;
	
	public:
		typedef T argument_type;
		typedef double result_type;

		explicit add(double y) : y(y) {}
		double operator()(T x) const { return x + y; }
	};

	template <class T>
	class subtract {
		T y;
	public:
		typedef T argument_type;
		typedef double result_type;

		explicit subtract(double y) : y(y) {}
		double operator()(T x) const { return x - y; }
	};

	template <class T>
	class subtract_from {
		T y;
	public:
		typedef T argument_type;
		typedef double result_type;

		explicit subtract_from(double y) : y(y) {}
		double operator()(T x) const { return y - x; }
	};

	template <class T>
	class multiply_by {
		T y;
	public:
		typedef T argument_type;
		typedef double result_type;

		explicit multiply_by(double y) : y(y) {}
		double operator()(T x) const { return x * y; }
	};

	template <class T>
	class divide_by {
		T y;
	public:
		typedef T argument_type;
		typedef double result_type;

		explicit divide_by(double y) : y(y) {}
		double operator()(T x) const { return x / y; }
	};

	template <class T>
	class less_than {
		T y;
	public:
		typedef T argument_type;
		typedef bool result_type;

		explicit less_than(double y) : y(y) {}
		bool operator()(T x) const { return x < y; }
	};

	template <class T>
	class greater_than {
		T y;
	public:
		typedef T argument_type;
		typedef bool result_type;

		explicit greater_than(double y) : y(y) {}
		bool operator()(T x) const { return x > y; }
	};

	template <class T>
	class greater_or_equal_to {
		T y;
	public:
		typedef T argument_type;
		typedef bool result_type;

		explicit greater_or_equal_to(double y) :y(y) {}
		bool operator()(T x) const { return x >= y; }
	};

	template <class T>
	class not_zero {
	public:
		typedef T argument_type;
		typedef bool result_type;
		bool operator()(T x) const { return x != T(); }
	};

	template <class T>
	class not_null {
		T null;
	public:
		typedef T argument_type;
		typedef bool result_type;

		not_null() : null(Null<T>()) {}
		bool operator()(T x) const { return x != null; }
	};

	class everywhere : public constant<double, bool> {
	public:
		everywhere() : constant<double, bool>(true) {}
	};

	template <class T>
	class nowhere : public constant<double, bool> {
		nowhere() : constant<double, bool>(false) {}
	};

	template <class T>
	class equal_within {
	public:
		typedef T first_argument_type;
		typedef T second_argument_type;
		typedef bool result_type;

		explicit equal_within(const T& eps) : eps_(eps) {}
		bool operator()(const T& a, const T& b) const { return std::fabs(a - b) <= eps_; }

	private:
		const T eps_;
	};

	template <class F, class R>
	class clipped_function {
	public:
		typedef typename F::argument_type argument_type;
		typedef typename F::result_type result_type;
		clipped_function(const F& f, const R& r) : f_(f), r_(r) {}
		result_type operator()(const argument_type& x) const { return r_(x) ? f_(x) : result_type(); }

	private:
		F f_;
		R r_;
	};

	template <class F, class R>
	clipped_function<F, R> clip(const F& f, const R& r)
	{
		return clipped_function<F, R>(f, r);
	}

	template <class F, class G>
	class composed_function {
	public:
		typedef typename G::argument_type argument_type;
		typedef typename F::result_type result_type;
		composed_function(const F& f, const G& g) :f_(f), g_(g) {}
		result_type operator()(const argument_type& x) const { return f_(g_(x)); }

	private:
		F f_;
		G g_;
	};

	template <class F, class G>
	composed_function<F, G> compose(const F& f, const G& g)
	{
		return composed_function<F, G>(f, g);
	}

	template <class F, class G, class H>
	class binary_compose3_function {
	public:
		typedef typename G::argument_type first_argument_type;
		typedef typename H::argument_type second_argument_type;
		typedef typename F::argument_type result_type;

		binary_compose3_function(const F& f, const G& g, const H& h): f_(f), g_(g), h_(h) {}
		
		result_type operator()(const first_argument_type& x, const second_argument_type& y) const { return f_(g_(x), h_(y)); }

	private:
		F f_;
		G g_;
		H h_;
	};

	template <class F, class G, class H>
	binary_compose3_function<F, G, H> compose3(const F& f, const G& g, const H& h)
	{
		return binary_compose3_function<F, G, H>(f, g, h);
	}

}