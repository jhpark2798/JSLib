#include "Payoff.h"
#include <algorithm>

namespace JSLib {

	double PlainVanillaPayoff::operator()(double price) const {
		return std::max(type_ * (price - strike_), 0.0);
	}

}