#pragma once

#include "TermStructure.h"
#include "..//Time/DayCounter.h"
#include "..//Time/Calendar.h"
#include "..//Time/Date.h"

namespace JSLib {

	// 간단한 코드를 위해 businessDayConvention, Period 생략
	class VolatilityTermStructure : public TermStructure {
	public:
		VolatilityTermStructure(const DayCounter& dc = DayCounter());
		VolatilityTermStructure(const Date& referenceDate,
			const Calendar& cal, const DayCounter& dc = DayCounter());
		VolatilityTermStructure(int settlementDays,
			const Calendar& cal, const DayCounter& dc = DayCounter());
		Date optionDateFromTenor(const int p) const;	// 무슨 역할?
		virtual double minStrike() const = 0;
		virtual double maxStrike() const = 0;
	protected:
		void checkStrike(double strike, bool extrapolate) const;
	};

}