#pragma once

#include "Interpolation.h"

#include <vector>

namespace JSLib {

	// Interpolation의 Impl과 templateImpl은 protected로 해놓았는데
	// 왜 LinearInterpolation의 LinearInterpolationImpl은 밖으로 빼놓았을까?
	template <class I1, class I2>
	class LinearInterpolationImpl : public Interpolation::templateImpl<I1, I2> {
	public:
		LinearInterpolationImpl(const I1& xBegin, const I1& xEnd, const I2& yBegin)
			: Interpolation::templateImpl<I1, I2>(xBegin, xEnd, yBegin),
			primitiveConst_(xEnd - xBegin), s_(xEnd - xBegin) {}	// primitive, s 초기화 왜 저렇게 함?
		
		void update() {
			primitiveConst_[0] = 0.0;
			// 왜 s_랑 primitiveConst_ index를 다르게 해서 헷갈리게 했을까?
			// s_의 맨 마지막 원소는 초기화가 안됬는데 왜 그냥 놔둘까?
			for (size_t i = 1; i < size_t(this->xEnd_ - this->xBegin_); ++i) {
				double dx = this->xBegin_[i] - this->xBegin_[i - 1];	// 구간 길이
				s_[i - 1] = (this->yBegin_[i] - this->yBegin_[i - 1]) / dx;	// 구간 기울기
				primitiveConst_[i] = primitiveConst_[i - 1] 
					+ dx * (this->yBegin_[i - 1] + 0.5 * dx * s_[i - 1]);	// 구간 누적 넓이
			}
		}

		double value(double x) const {
			size_t i = this->locate(x);
			return this->yBegin_[i] + (x - this->xBegin_[i]) * s_[i];	// linear interpolation scheme
		}
		
		// 정적분 값이 필요한 경우가 있나?
		double primitive(double x) const { // 0부터 x까지의 정적분 값
			size_t i = this->locate(x);
			double dx = x - this->xBegin_[i];
			return primitiveConst_[i] + dx * (this->yBegin_[i] + 0.5 * dx * s_[i]);
		}
		
		double derivative(double x) const {
			size_t i = this->locate(x);
			return s_[i];
		}

	private:
		std::vector<double> primitiveConst_;	// 무슨 역할?	// 구간별 넓이 // 적분이라 primitive  인듯
		std::vector<double> s_;	// 무슨 역할?	// 구간별 기울기?
	};	

	class LinearInterpolation : public Interpolation {
	public:
		template <class I1, class I2>
		LinearInterpolation(const I1& xBegin, const I1& xEnd, const I2& yBegin) {
			impl_ = std::shared_ptr<Interpolation::Impl>(new LinearInterpolationImpl<I1, I2>(xBegin,
				xEnd, yBegin));
			impl_->update();	// 왜 update?
		}
	};

	// LinearInterpolation의 traits class // Interpolator 라고 생각
	// Interpolation 객체를 가지고 있는 객체가 복사생성자를 작성할 때 interpolate 함수를 사용해야 함
	// 이와 같이 LinearInterpolation의 부수적인 기능들을 모아놓은 클래스인듯
	class Linear {	
	public:
		template <class I1, class I2>
		Interpolation interpolate(const I1& xBegin, const I1& xEnd, const I2& yBegin) const {
			return LinearInterpolation(xBegin, xEnd, yBegin);
		}
		static const bool global = false;	// interpolation의 point가 변하면 그 사이만 변하는지 아닌지
		static const size_t requiredPoints = 2;	// interpolation에 필요한 점 갯수
	};

}