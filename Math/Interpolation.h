#pragma once

#include "Extrapolator.h"

#include <memory>
#include <algorithm>

namespace JSLib {
	
	class Interpolation : public Extrapolator {
	protected:
		class Impl {
		public:
			virtual ~Impl() {}
			virtual void update() = 0;	// 뭔 함수?
			virtual double xMin() const = 0;
			virtual double xMax() const = 0;
			virtual double value(double) const = 0;
			virtual bool isInRange(double) const = 0;
			virtual double primitive(double) const = 0;	// 뭔 함수?
			virtual double derivative(double) const = 0;	// 뭔 함수?
		};

		// 그냥 애초에 Impl을 template으로 만들면 되지 왜 Impl 만들고 상속받음?
		template <class I1, class I2>
		class templateImpl : public Impl {
		public:
			templateImpl(const I1& xBegin, const I1& xEnd, const I2& yBegin)
				: xBegin_(xBegin), xEnd_(xEnd), yBegin_(yBegin) {}
			double xMin() const { return *xBegin_; }
			double xMax() const { return *(xEnd_ - 1); }
			bool isInRange(double x) const { return x >= xMin() && x <= xMax(); }
		protected:
			size_t locate(double x) const {
				if (x < *xBegin_) return 0;
				else if (x > * (xEnd_ - 1)) return xEnd_ - xBegin_ - 2; // xEnd_-1 아닌가? -xBegin_-1은 왜 해줄까?
				else return std::upper_bound(xBegin_, xEnd_ - 1, x) - xBegin_ - 1; // 뒤에 -xBegin_-1은 왜 해줄까?
			}
			I1 xBegin_, xEnd_;
			I2 yBegin_;
		};

		std::shared_ptr<Impl> impl_;
		void checkRange(double x, bool extrapolate) const;
	public:
		Interpolation() {}
		virtual ~Interpolation() {}
		typedef double argument_type;	// 어따 씀?
		typedef double result_type;	// 어따 씀?
		bool empty() const { return !impl_; }
		double operator()(double x, bool extrapolate = false) const;
		double primitive(double x, bool extrapolate = false) const;
		double derivative(double x, bool extrapolate = false) const;
		double xMin() const { return impl_->xMin(); }
		double xMax() const { return impl_->xMax(); }
		void update() { impl_->update(); }
	};

}