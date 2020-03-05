#pragma once

#include "Quote.h"
#include "Utility.h"

namespace JSLib {

	class SimpleQuote : public Quote {
	public:
		SimpleQuote(double value = NULL_DOUBLE) : value_(value) {}
		double value() const { // Quote interface
			JS_REQUIRE(isValid(), "invalid SimpleQuote");
			return value_;
		}
		bool isValid() const {	// Quote interface
			return value_ != NULL_DOUBLE;
		}
		double setValue(double value = NULL_DOUBLE) {
			double diff = value - value_;
			if (diff != 0.0) {
				value_ = value;
				notifyObservers();
			}
			return diff;
		}
		void reset() {
			setValue(NULL_DOUBLE);
		}
	private:
		double value_;
	};

}