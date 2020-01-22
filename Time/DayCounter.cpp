#include "DayCounter.h"

namespace JSLib {

	double DayCounter::yearFraction(const Date& d1, const Date& d2) const {
		JS_REQUIRE(impl_, "no day counter implementation provided");
		return impl_->yearFraction(d1, d2);
	}

}