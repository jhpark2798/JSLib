#pragma once

#include "Interpolation.h"

#include <vector>

namespace JSLib {

	// Interpolation�� Impl�� templateImpl�� protected�� �س��Ҵµ�
	// �� LinearInterpolation�� LinearInterpolationImpl�� ������ ����������?
	template <class I1, class I2>
	class LinearInterpolationImpl : public Interpolation::templateImpl<I1, I2> {
	public:
		LinearInterpolationImpl(const I1& xBegin, const I1& xEnd, const I2& yBegin)
			: Interpolation::templateImpl<I1, I2>(xBegin, xEnd, yBegin),
			primitiveConst_(xEnd - xBegin), s_(xEnd - xBegin) {}	// primitive, s �ʱ�ȭ �� ������ ��?
		
		void update() {
			primitiveConst_[0] = 0.0;
			// �� s_�� primitiveConst_ index�� �ٸ��� �ؼ� �򰥸��� ������?
			// s_�� �� ������ ���Ҵ� �ʱ�ȭ�� �ȉ�µ� �� �׳� ���ѱ�?
			for (size_t i = 1; i < size_t(this->xEnd_ - this->xBegin_); ++i) {
				double dx = this->xBegin_[i] - this->xBegin_[i - 1];	// ���� ����
				s_[i - 1] = (this->yBegin_[i] - this->yBegin_[i - 1]) / dx;	// ���� ����
				primitiveConst_[i] = primitiveConst_[i - 1] 
					+ dx * (this->yBegin_[i - 1] + 0.5 * dx * s_[i - 1]);	// ���� ���� ����
			}
		}

		double value(double x) const {
			size_t i = this->locate(x);
			return this->yBegin_[i] + (x - this->xBegin_[i]) * s_[i];	// linear interpolation scheme
		}
		
		// ������ ���� �ʿ��� ��찡 �ֳ�?
		double primitive(double x) const { // 0���� x������ ������ ��
			size_t i = this->locate(x);
			double dx = x - this->xBegin_[i];
			return primitiveConst_[i] + dx * (this->yBegin_[i] + 0.5 * dx * s_[i]);
		}
		
		double derivative(double x) const {
			size_t i = this->locate(x);
			return s_[i];
		}

	private:
		std::vector<double> primitiveConst_;	// ���� ����?	// ������ ���� // �����̶� primitive  �ε�
		std::vector<double> s_;	// ���� ����?	// ������ ����?
	};	

	class LinearInterpolation : public Interpolation {
	public:
		template <class I1, class I2>
		LinearInterpolation(const I1& xBegin, const I1& xEnd, const I2& yBegin) {
			impl_ = std::shared_ptr<Interpolation::Impl>(new LinearInterpolationImpl<I1, I2>(xBegin,
				xEnd, yBegin));
			impl_->update();	// �� update?
		}
	};

	// LinearInterpolation�� traits class // Interpolator ��� ����
	// Interpolation ��ü�� ������ �ִ� ��ü�� ��������ڸ� �ۼ��� �� interpolate �Լ��� ����ؾ� ��
	// �̿� ���� LinearInterpolation�� �μ����� ��ɵ��� ��Ƴ��� Ŭ�����ε�
	class Linear {	
	public:
		template <class I1, class I2>
		Interpolation interpolate(const I1& xBegin, const I1& xEnd, const I2& yBegin) const {
			return LinearInterpolation(xBegin, xEnd, yBegin);
		}
		static const bool global = false;	// interpolation�� point�� ���ϸ� �� ���̸� ���ϴ��� �ƴ���
		static const size_t requiredPoints = 2;	// interpolation�� �ʿ��� �� ����
	};

}