#pragma once

#include "ZeroYieldStructure.h"
#include "..//Time/Date.h"
#include "..//Math/Interpolation.h"
#include "..//Utility.h"

#include <vector>

namespace JSLib {

	// Interpolator를 생성자에서 주는데 왜 템플릿에서 타입을 알려줘야 하도록 만들었을까?
	// Linear 클래스의 기본 클래스가 없어서 이런 종류의 클래스를 한번에 받는 타입이 없긴 한데
	// 애초에 그렇게 클래스를 설계했다면 템플릿을 안쓰고도 받을 수 있을텐데
	// 그렇게 설계하기가 힘들어서 이렇게 한건가?
	// LinearInterpolation::Linear 이런식으로 짜면 안되나? Interpolation::Interpolator 상속받고
	// 이렇게 템플릿으로 했다가 Linear 를 안넣고 이상한걸 넣어버리면 어떻게 해
	// 제대로 쓰기엔 어렵게, 잘못 쓰기엔 쉽게 만든 것 같음
	template <class Interpolator>
	class InterpolatedZeroCurve : public ZeroYieldStructure {
	public:
		InterpolatedZeroCurve(const std::vector<Date>& dates,
			const std::vector<double>& yields, const DayCounter& dayCounter,
			const Interpolator& interpolator = Interpolator());
		Date maxDate() const { return dates_.back(); }
	protected:
		double zeroYieldImpl(double t) const;

		// 왜 mutable? // 파생클래스에서 데이터를 변경할 수 있게 하기 위함이라 함.
		// 파생클래스의 const 함수에서 바꿔야 하는 경우가 생기나?
		mutable std::vector<Date> dates_;
		mutable std::vector<double> times_;	// 굳이 time을 또? // interpolation하려면 double이어야 함
		mutable std::vector<double> yields_;
		mutable Interpolation interpolation_;
		Interpolator interpolator_;

	private:
		// dates와 yields를 받는 생성자에서 공통적으로 호출되는 함수
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
		// flat fwd extrapolation 이라는데 로직을 잘 모르겠음.
		// 그리고 왜 extrapolation을 여기서 처리해줄까? 이럴거면 interpolation에서
		// bool extrapolation을 왜받음? extrapolation을 true로 주면
		// interpolation 클래스에서 처리해야 하는 것이 아닌가?
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
		// 원래는 Compounding, Frequency를 input으로 받아서
		// compounding이 Continuous가 아닌 경우는 따로 처리해줘야 하지만 생략
		for (size_t i = 1; i < dates_.size(); ++i) {
			JS_REQUIRE(dates_[i] > dates_[i - 1], "invalid date");
			times_[i] = dayCounter().yearFraction(dates_[0], dates_[i]);
		}
		interpolation_ = interpolator_.interpolate(times_.begin(), times_.end(), yields_.begin());
		interpolation_.update();
	}

}