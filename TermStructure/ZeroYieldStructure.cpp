#include "ZeroYieldStructure.h"

#include <cmath>

namespace JSLib {

	double ZeroYieldStructure::discountImpl(double t) const {
		double r = zeroYieldImpl(t);
		return std::exp(-r * t);
	}

}