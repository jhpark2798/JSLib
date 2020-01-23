#pragma once

#include "YieldTermStructure.h"

namespace JSLib {

	class ZeroYieldStructure : public YieldTermStructure {
	public:
		ZeroYieldStructure(const DayCounter& dc = DayCounter())
			: YieldTermStructure(dc) {}
		ZeroYieldStructure(const Date& referenceDate,
			const Calendar& calendar = Calendar(),
			const DayCounter& dc = DayCounter())
			: YieldTermStructure(referenceDate, calendar, dc) {}
		ZeroYieldStructure(int settlementDays,
			const Calendar& calendar, const DayCounter& dc = DayCounter())
			: YieldTermStructure(settlementDays, calendar, dc) {}

	protected:
		virtual double zeroYieldImpl(double t) const = 0;
		double discountImpl(double t) const;
	};

}