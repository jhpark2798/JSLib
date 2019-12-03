#include "TermStructure.h"
#include "..//Utility.h"
#include "..//Settings.h"

namespace JSLib {

	TermStructure::TermStructure(const DayCounter& dc) 
	: moving_(false), updated_(true), 
		settlementDays_(NULL_INT), dayCounter_(dc)	{}

	TermStructure::TermStructure(const Date& referenceDate,
		const Calendar& calendar, const DayCounter& dc)
		: moving_(false), updated_(true),
		referenceDate_(referenceDate), settlementDays_(NULL_INT),
		calendar_(calendar), dayCounter_(dc) {}

	TermStructure::TermStructure(int settlementDays,
		const Calendar& calendar, const DayCounter& dc)
		: moving_(true), updated_(false),
		settlementDays_(settlementDays), calendar_(calendar), dayCounter_(dc) {
		registerWith(Settings::instance().evaluationDate());
	}

	double TermStructure::maxTime() const {
		return timeFromReference(maxDate());
	}

	const Date& TermStructure::referenceDate() const {
		if (!updated_) {
			Date today = Settings::instance().evaluationDate();
			referenceDate_ = calendar().advance(today, settlementDays());
			updated_ = true;
		}
		return referenceDate_;
	}

	double TermStructure::timeFromReference(const Date& d) const {
		return dayCounter().yearFraction(referenceDate(), d);
	}

	void TermStructure::update() {
		if (moving_)
			updated_ = false;
		notifyObservers();
	}

	void TermStructure::checkRange(const Date& date, bool extrapolate) const {
		checkRange(timeFromReference(date), extrapolate);
	}

	void TermStructure::checkRange(double time, bool extrapolate) const {
		JS_REQUIRE(time >= 0.0, "negative time given");
		JS_REQUIRE(extrapolate || allowsExtrapolation() || time <= maxTime(),
			"time is past max curve time");
	}

}




