#pragma once

#include <limits>
#include <stdexcept>
#include <cmath>

#define NULL_DOUBLE std::numeric_limits<double>::quiet_NaN()
#define NULL_INT std::numeric_limits<int>::quiet_NaN()

#define JS_EPSILON ((std::numeric_limits<double>::epsilon)())

#define JS_REQUIRE(condition,message) \
if (!(condition)) { \
	throw std::runtime_error(message);\
} else

#define JS_FAIL(message) (throw std::runtime_error(message));

#define DAY_OF_YEAR 365.0

namespace JSLib {

	bool close_enough(double x, double y, std::size_t n);
	bool close_enough(double x, double y);

	bool close(double x, double y);
	bool close(double x, double y, std::size_t n);

	inline bool close_enough(double x, double y, std::size_t n) {
		if (x == y) return true;
		double diff = std::abs(x - y);
		double tolerance = n * JS_EPSILON;
		if (x * y == 0.0) return diff < (tolerance * tolerance);
		return diff <= tolerance * std::abs(x) || diff <= tolerance * std::abs(y);
	}

	inline bool close_enough(double x, double y) {
		return close_enough(x, y, 42);
	}

	inline bool close(double x, double y) {
		return close(x, y, 42);
	}

	inline bool close(double x, double y, std::size_t n) {
		if (x == y) return true;
		auto diff = std::abs(x - y), tolerance = n * JS_EPSILON;
		if (x * y == 0) return diff < (tolerance * tolerance);
		return diff <= tolerance * std::abs(x) && diff <= tolerance * std::abs(y);
	}
}

