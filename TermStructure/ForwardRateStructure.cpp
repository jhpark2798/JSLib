#include "ForwardRateStructure.h"

#include <cmath>

namespace JSLib {

	double ForwardRateStructure::zeroYieldImpl(double t) const {
		// averages forwardImpl between 0 and t
		// have to implement numerically later
		return NULL_DOUBLE;
	}
	
	double ForwardRateStructure::discountImpl(double t) const {
		double r = zeroYieldImpl(t);
		return std::exp(-r * t);
	}

}