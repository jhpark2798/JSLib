#include "InterestRate.h"
#include "..//Utility.h"

#include <cmath>

namespace JSLib {
	
	InterestRate::InterestRate() {

	}

	InterestRate::InterestRate(double r, const DayCounter& dc,
		Compounding comp, Frequency freq)
		: r_(r), dc_(dc), comp_(comp), freqMakesSense_(false) {
		if (comp_ == Compounded) {
			freqMakesSense_ = true;
			JS_REQUIRE(freq != Once && freq != NoFrequency, "invalid frequency");
			freq_ = double(freq);
		}
	}

	double InterestRate::discountFactor(double t) const {
		return 1.0 / compoundFactor(t);
	}

	double InterestRate::discountFactor(const Date& d1,
		const Date& d2) const {
		JS_REQUIRE(d2 >= d1, "d1 is later than d2");
		double t = dc_.yearFraction(d1, d2);
		return discountFactor(t);
	}

	double InterestRate::compoundFactor(double t) const {
		switch (comp_) {
		case Simple:
			return 1.0 + r_ * t;
		case Compounded:
			return std::pow(1.0 + r_ / freq_, freq_ * t);
		case Continuous:
			return std::exp(r_ * t);
		default:
			JS_REQUIRE(true, "unknown compounding convention");
			return NULL_DOUBLE;
		}
	}

	double InterestRate::compoundFactor(const Date& d1, const Date& d2) const {
		JS_REQUIRE(d2 >= d1, "d1 is later than d2");
		double t = dc_.yearFraction(d1, d2);
		return compoundFactor(t);
	}

	InterestRate InterestRate::impliedRate(double compound,
		const DayCounter& dc, Compounding comp, Frequency freq, double t) {
		JS_REQUIRE(compound > 0.0, "positive compound factor required");
		double r;
		if (compound == 1.0) {
			JS_REQUIRE(t >= 0.0, "non-negative time required");
			r = 0.0;
		}
		else {
			JS_REQUIRE(t > 0, "positive time required");
			switch (comp) {
			case Simple:
				r = (compound - 1.0) / t;
				break;
			case Compounded:
				r = double(freq) * (std::pow(compound, 1.0 / (double(freq) * t)) - 1.0);
				break;
			case Continuous:
				r = std::log(compound) / t;
				break;
			default:
				JS_REQUIRE(true, "unknown compounding convention");
			}
		}
		return InterestRate(r, dc, comp, freq);
	}

	InterestRate InterestRate::impliedRate(double compound,
		const DayCounter& dc, Compounding comp, Frequency freq,
		const Date& d1, const Date& d2) {
		JS_REQUIRE(d2 >= d1, "d1 is later than d2");
		double t = dc.yearFraction(d1, d2);
		return impliedRate(compound, dc, comp, freq, t);
	}

}