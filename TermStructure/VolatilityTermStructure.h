#pragma once

#include "TermStructure.h"
#include "..//Time/DayCounter.h"
#include "..//Time/Calendar.h"
#include "..//Time/Date.h"

namespace JSLib {

	// ������ �ڵ带 ���� businessDayConvention, Period ����
	class VolatilityTermStructure : public TermStructure {
	public:
		VolatilityTermStructure(const DayCounter& dc = DayCounter());
		VolatilityTermStructure(const Date& referenceDate,
			const Calendar& cal, const DayCounter& dc = DayCounter());
		VolatilityTermStructure(int settlementDays,
			const Calendar& cal, const DayCounter& dc = DayCounter());
		Date optionDateFromTenor(const int p) const;	// ���� ����?
		virtual double minStrike() const = 0;
		virtual double maxStrike() const = 0;
	protected:
		void checkStrike(double strike, bool extrapolate) const;
	};

}