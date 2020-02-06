#include "VolatilityTermStructure.h"
#include "..//Utility.h"

namespace JSLib {

	VolatilityTermStructure::VolatilityTermStructure(const DayCounter& dc)
		: TermStructure(dc) {}

	VolatilityTermStructure::VolatilityTermStructure(const Date& referenceDate,
		const Calendar& cal, const DayCounter& dc)
		: TermStructure(referenceDate, cal, dc) {}

	VolatilityTermStructure::VolatilityTermStructure(int settlementDays,
		const Calendar& cal, const DayCounter& dc)
		: TermStructure(settlementDays, cal, dc) {}

	Date VolatilityTermStructure::optionDateFromTenor(const int p) const {
		// swaption style
		return calendar().advance(referenceDate(), p);
	}

	void VolatilityTermStructure::checkStrike(double strike, bool extrapolate) const {
		JS_REQUIRE(extrapolate || allowsExtrapolation() ||
			(strike >= minStrike() && strike <= maxStrike()), "invalid strike"
		);
	}

}