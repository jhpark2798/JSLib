#pragma once

#include "ZeroYieldStructure.h"
#include "..//Time/Date.h"
#include "..//Math/Interpolation.h"
#include "..//Utility.h"

#include <vector>

namespace JSLib {

	// Interpolator�� �����ڿ��� �ִµ� �� ���ø����� Ÿ���� �˷���� �ϵ��� ���������?
	// Linear Ŭ������ �⺻ Ŭ������ ��� �̷� ������ Ŭ������ �ѹ��� �޴� Ÿ���� ���� �ѵ�
	// ���ʿ� �׷��� Ŭ������ �����ߴٸ� ���ø��� �Ⱦ��� ���� �� �����ٵ�
	// �׷��� �����ϱⰡ ���� �̷��� �Ѱǰ�?
	// LinearInterpolation::Linear �̷������� ¥�� �ȵǳ�? Interpolation::Interpolator ��ӹް�
	// �̷��� ���ø����� �ߴٰ� Linear �� �ȳְ� �̻��Ѱ� �־������ ��� ��
	// ����� ���⿣ ��ư�, �߸� ���⿣ ���� ���� �� ����
	template <class Interpolator>
	class InterpolatedZeroCurve : public ZeroYieldStructure {
	public:
		InterpolatedZeroCurve(const std::vector<Date>& dates,
			const std::vector<double>& yields, const DayCounter& dayCounter,
			const Interpolator& interpolator = Interpolator());
		Date maxDate() const { return dates_.back(); }
	protected:
		double zeroYieldImpl(double t) const;

		// �� mutable? // �Ļ�Ŭ�������� �����͸� ������ �� �ְ� �ϱ� �����̶� ��.
		// �Ļ�Ŭ������ const �Լ����� �ٲ�� �ϴ� ��찡 ���⳪?
		mutable std::vector<Date> dates_;
		mutable std::vector<double> times_;	// ���� time�� ��? // interpolation�Ϸ��� double�̾�� ��
		mutable std::vector<double> yields_;
		mutable Interpolation interpolation_;
		Interpolator interpolator_;

	private:
		// dates�� yields�� �޴� �����ڿ��� ���������� ȣ��Ǵ� �Լ�
		void initialize();	
	};

	template <class T>
	InterpolatedZeroCurve<T>::InterpolatedZeroCurve(const std::vector<Date>& dates,
		const std::vector<double>& yields, const DayCounter& dayCounter,
		const T& interpolator)
		: ZeroYieldStructure(dates.front(), Calendar(), dayCounter),
		dates_(dates), yields_(yields), interpolator_(interpolator) {
		initialize();
	}

	template <class T>
	double InterpolatedZeroCurve<T>::zeroYieldImpl(double t) const {
		if (t <= times_.back()) return interpolation_(t, true);
		// flat fwd extrapolation �̶�µ� ������ �� �𸣰���.
		// �׸��� �� extrapolation�� ���⼭ ó�����ٱ�? �̷��Ÿ� interpolation����
		// bool extrapolation�� �ֹ���? extrapolation�� true�� �ָ�
		// interpolation Ŭ�������� ó���ؾ� �ϴ� ���� �ƴѰ�?
		double tMax = times_.back();
		double zMax = yields_.back();
		double instFwdMax = zMax + tMax * interpolation_.derivative(tMax);
		return (zMax * tMax + instFwdMax * (t - tMax)) / t;
	}

	template <class T>
	void InterpolatedZeroCurve<T>::initialize() {
		JS_REQUIRE(dates_.size() >= T::requiredPoints, "not enough input data given");
		JS_REQUIRE(yields_.size() == dates_.size(), "yields/dates data mismatch");
		times_.resize(dates_.size());
		times_[0] = 0.0;
		// ������ Compounding, Frequency�� input���� �޾Ƽ�
		// compounding�� Continuous�� �ƴ� ���� ���� ó������� ������ ����
		for (size_t i = 1; i < dates_.size(); ++i) {
			JS_REQUIRE(dates_[i] > dates_[i - 1], "invalid date");
			times_[i] = dayCounter().yearFraction(dates_[0], dates_[i]);
		}
		interpolation_ = interpolator_.interpolate(times_.begin(), times_.end(), yields_.begin());
		interpolation_.update();
	}

}