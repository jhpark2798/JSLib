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
		// 미완성
		return 0;
	}

	const Date& TermStructure::referenceDate() const {
		// 미완성
		return Date();
	}

	double TermStructure::timeFromReference(const Date& date) const {
		// 미완성
		return 0;
	}

	void TermStructure::update() {
		// 미완성
	}

	void TermStructure::checkRange(const Date& date, bool extrapolate) const {
		// 미완성
	}

	void TermStructure::checkRange(double time, bool extrapolate) const {
		// 미완성
	}

}