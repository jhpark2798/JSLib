#pragma once

#include "YieldTermStructure.h"
#include "..//Time/DayCounter.h"

namespace JSLib {

	class ForwardRateStructure : public YieldTermStructure {
	public:
		ForwardRateStructure(const DayCounter& dc = DayCounter())
			: YieldTermStructure(dc) {}
		ForwardRateStructure(const Date& referenceDate,
			const Calendar& calendar = Calendar(),
			const DayCounter& dc = DayCounter())
			: YieldTermStructure(referenceDate, calendar, dc) {}
		ForwardRateStructure(int settlementDays,
			const Calendar& calendar, const DayCounter& dc = DayCounter())
			: YieldTermStructure(settlementDays, calendar, dc) {}
	private:
		virtual double forwardImpl(double t) const = 0;
		virtual double zeroYieldImpl(double t) const;
		double discountImpl(double t) const;
	};

}