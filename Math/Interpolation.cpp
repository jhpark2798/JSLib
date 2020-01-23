#include "Interpolation.h"

#include "..//Utility.h"

namespace JSLib {

	void Interpolation::checkRange(double x, bool extrapolate) const {
		JS_REQUIRE(extrapolate || allowsExtrapolation() || impl_->isInRange(x),
			"invalid interpolation range");
	}

	double Interpolation::operator()(double x, bool extrapolate) const {
		checkRange(x, extrapolate);
		return impl_->value(x);
	}

	double Interpolation::primitive(double x, bool extrapolate) const {
		checkRange(x, extrapolate);
		return impl_->primitive(x);
	}

	double Interpolation::derivative(double x, bool extrapolate) const {
		checkRange(x, extrapolate);
		return impl_->derivative(x);
	}

}