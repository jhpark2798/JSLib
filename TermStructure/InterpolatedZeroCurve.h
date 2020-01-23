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
			// date�� ���ĵǾ� �ִ��� Ȯ��. �ȵ������� ���� �ٲ�. �� �� yields ���� �͵鵵 �ٲ�� ��
			// dates�� times�� ��ȯ.
			interpolation_ = interpolator_.interpolate(times_.begin(), times_.end(), yields_.begin());
		}
	
	protected:
		double zeroYieldImpl(double t) const;

		// �� mutable?
		mutable std::vector<Date> dates_;
		mutable std::vector<double> times_;	// ���� time�� ��?
		mutable std::vector<double> yields_;
		mutable Interpolation interpolation_;
		Interpolator interpolator_;
	};

}