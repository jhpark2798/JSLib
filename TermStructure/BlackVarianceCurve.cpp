#include "BlackVarianceCurve.h"
#include "..//Math/LinearInterpolation.h"
#include "..//Utility.h"

namespace JSLib {

	BlackVarianceCurve::BlackVarianceCurve(
		const Date& referenceDate,
		const std::vector<Date>& dates,
		const std::vector<double>& blackVolCurve,
		const DayCounter& dc, const Calendar& cal)
		: BlackVarianceTermStructure(referenceDate, cal, dc), 
		maxDate_(dates.back()) {
		JS_REQUIRE(dates.size() == blackVolCurve.size(),
			"size mismatch between date vector and black vol vector");
		JS_REQUIRE(dates[0] > referenceDate,
			"cannot have dates[0]<=referenceDate");
		variances_ = std::vector<double>(dates.size() + 1);
		times_ = std::vector<double>(dates.size() + 1);
		variances_[0] = 0.0;
		times_[0] = 0.0;
		for (int j = 1; j <= blackVolCurve.size(); ++j) {
			times_[j] = timeFromReference(dates[j - 1]);
			JS_REQUIRE(times_[j] > times_[j - 1], "dates must be sorted unique");
			variances_[j] = blackVolCurve[j-1] * blackVolCurve[j-1] * times_[j];
		}
		setInterpolation<Linear>();
	}

	Date BlackVarianceCurve::maxDate() const {
		return maxDate_;
	}

	// is a 관계가 아니라 나타나는 문제
	double BlackVarianceCurve::minStrike() const {
		return -1;
	}

	double BlackVarianceCurve::maxStrike() const {
		return 999999;
	}

	double BlackVarianceCurve::blackVarianceImpl(double t, double) const {
		JS_REQUIRE(t >= 0, "t must be greater than or equal to 0");
		if (t <= times_.back() && t >= 0) 
			return varianceCurve_(t, true);
		else
			varianceCurve_(times_.back(), true)* t / times_.back();
	}

}