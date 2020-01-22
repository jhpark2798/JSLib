#pragma once

#include "TermStructure.h"
#include "..//Time/DayCounter.h"
#include "..//Time/Calendar.h"
#include "InterestRate.h"

namespace JSLib {

	class YieldTermStructure : public TermStructure {
	public:
		YieldTermStructure(const DayCounter& dc = DayCounter());
		YieldTermStructure(const Date& referenceDate,
			const Calendar& calendar = Calendar(),
			const DayCounter& dc = DayCounter());
		YieldTermStructure(int settlementDays,	// today�κ��� ��������. �׸��� �� ���ϸ�ŭ
			const Calendar&, const DayCounter& dc = DayCounter()); // ���� �ϴϱ� calendar�� �ʼ�
		
		InterestRate zeroRate(const Date&, const DayCounter&,
			Compounding, Frequency freq=Annual, bool extrapolate=false) const;	// extrapolate�� �� �ʿ�?
		InterestRate zeroRate(double t, Compounding,
			Frequency freq = Annual, bool extrapolate = false) const;
		double discount(const Date&, bool extrapolate = false) const;
		double discount(double t, bool extrapolate = false) const;
		InterestRate forwardRate(const Date& d1, const Date& d2,
			const DayCounter&, Compounding,
			Frequency freq = Annual, bool extrapolate = false) const;

	protected:
		virtual double discountImpl(double t) const = 0;
	};

}