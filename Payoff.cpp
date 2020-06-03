#include "Payoff.h"
#include <algorithm>

namespace JSLib {

	double PlainVanillaPayoff::operator()(double price) const {
		return std::max(static_cast<int>(type_) * (price - strike_), 0.0);
	}

}