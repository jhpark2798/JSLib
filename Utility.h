#pragma once

#include <limits>
#include <stdexcept>

#define NULL_DOUBLE std::numeric_limits<double>::quiet_NaN()
#define NULL_INT std::numeric_limits<int>::quiet_NaN()

#define JS_REQUIRE(condition,message) \
if (!(condition)) { \
	throw std::runtime_error(message);\
} else

#define DAY_OF_YEAR 365.0

namespace JSLib {

	enum OptionType { Call = 1, Put = -1 };

}

