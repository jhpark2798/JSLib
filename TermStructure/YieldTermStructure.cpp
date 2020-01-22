#include "YieldTermStructure.h"

#include <algorithm>

namespace JSLib {

	namespace {
		const double dt = 0.0001;
	}

	YieldTermStructure::YieldTermStructure(const DayCounter& dc) 
	: TermStructure(dc) {}
	
	YieldTermStructure::YieldTermStructure(const Date& referenceDate,
		const Calendar& calendar, const DayCounter& dc)
	: TermStructure(referenceDate, calendar, dc) {}

	YieldTermStructure::YieldTermStructure(int settlementDays,
		const Calendar& calendar, const DayCounter& dc) 
	: TermStructure(settlementDays, calendar, dc) {}

	InterestRate YieldTermStructure::zeroRate(const Date& d, 
		const DayCounter& dc, Compounding comp, 
		Frequency freq, bool extrapolate) const {
		if (d == referenceDate()) {
			double compound = 1.0 / discount(dt, extrapolate);	// compound = growth factor
			return InterestRate::impliedRate(compound, dc, comp, freq, dt);
		}
		double compound = 1.0 / discount(d, extrapolate);
		return InterestRate::impliedRate(compound, dc, comp, freq, referenceDate(), d);
	}
	
	InterestRate YieldTermStructure::zeroRate(double t, Compounding comp,
		Frequency freq, bool extrapolate) const {
		if (t == 0.0) t = dt;
		double compound = 1.0 / discount(t, extrapolate);
		return InterestRate::impliedRate(compound, dayCounter(), comp, freq, t);
	}

	double YieldTermStructure::discount(const Date& d, bool extrapolate) const {
		checkRange(d, extrapolate);
		return discountImpl(timeFromReference(d));
	}

	double YieldTermStructure::discount(double t, bool extrapolate) const {
		checkRange(t, extrapolate);
		return discountImpl(t);
	}

	InterestRate YieldTermStructure::forwardRate(const Date& d1, 
		const Date& d2,	const DayCounter& dc, Compounding comp,
		Frequency freq, bool extrapolate) const {
		if (d1 == d2) {
			checkRange(d1, extrapolate);
			double t1 = std::max(timeFromReference(d1) - dt / 2.0, 0.0);
			double t2 = t1 + dt;
			double compound = discount(t1, true) / discount(t2, true);
			return InterestRate::impliedRate(compound, dc, comp, freq, dt);
		}
		JS_REQUIRE(d1 < d2, "d1 is later than d2");
		double compound = discount(d1, extrapolate) / discount(d2, extrapolate);
		return InterestRate::impliedRate(compound, dc, comp, freq, d1, d2);
	}



}