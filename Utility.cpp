#include "Utility.h"

namespace JSLib {

	bool close_enough(double x, double y, std::size_t n) {
		if (x == y) return true;
		double diff = std::abs(x - y);
		double tolerance = n * JS_EPSILON;
		if (x * y == 0.0) return diff < (tolerance * tolerance);
		return diff <= tolerance * std::abs(x) || diff <= tolerance * std::abs(y);
	}

	bool close_enough(double x, double y) {
		return close_enough(x, y, 42);
	}
	
}