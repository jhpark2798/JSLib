#pragma once

#include "ZeroYieldStructure.h"
#include "..//Time/Date.h"
#include "..//Math/Interpolation.h"

#include <vector>

namespace JSLib {

	template <class Interpolator>
	class InterpolatedZeroCurve : public ZeroYieldStructure {
	public:
		InterpolatedZeroCurve(const std::vector<Date>& dates,
			const std::vector<double>& yields, const DayCounter& dayCounter,
			const Interpolator& interpolator = Interpolator())
			: ZeroYieldStructure(dates.front(), Calendar(), dayCounter),
			dates_(dates), yields_(yields), interpolator_(interpolator) {
			// date가 정렬되어 있는지 확인. 안되있으면 순서 바꿈. 이 때 yields 같은 것들도 바꿔야 함
			// dates를 times로 변환.
			interpolation_ = interpolator_.interpolate(times_.begin(), times_.end(), yields_.begin());
		}
	
	protected:
		double zeroYieldImpl(double t) const;

		// 왜 mutable?
		mutable std::vector<Date> dates_;
		mutable std::vector<double> times_;	// 굳이 time을 또?
		mutable std::vector<double> yields_;
		mutable Interpolation interpolation_;
		Interpolator interpolator_;
	};

}