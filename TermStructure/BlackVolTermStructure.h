#pragma once

#include "VolatilityTermStructure.h"
#include "..//Time/DayCounter.h"
#include "..//Time/Calendar.h"

namespace JSLib{

	class BlackVolTermStructure : public VolatilityTermStructure {
	public:
		BlackVolTermStructure(const DayCounter& dc=DayCounter())
			: VolatilityTermStructure(dc) {}
		BlackVolTermStructure(const Date& referenceDate,
			const Calendar& cal, const DayCounter& dc = DayCounter())
			: VolatilityTermStructure(referenceDate, cal, dc) {}
		BlackVolTermStructure(int settlementDays,
			const Calendar& cal, const DayCounter& dc = DayCounter())
			: VolatilityTermStructure(settlementDays, cal, dc) {}

		double blackVol(const Date& maturity, double strike,
			bool extrapolate = false) const;
		double blackVol(double maturity, double strike,
			bool extrapolate = false) const;
		double blackVariance(const Date& maturity, double strike,
			bool extrapolate = false) const;
		double blackVariance(double maturity, double strike,
			bool extrapolate = false) const;
		// forward vol 개념을 쓸 일이 없어보임
		//double blackForwardVol(const Date& date1,
		//	const Date& date2, double strike, bool extrapolate = false) const;
		//double blackForwardVariance(const Date& date1,
		//	const Date& date2, double strike, bool extrapolate = false) const;

	protected:
		virtual double blackVarianceImpl(double t, double strike) const = 0;
		virtual double blackVolImpl(double t, double strike) const = 0;
	};



}