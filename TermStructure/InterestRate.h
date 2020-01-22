#pragma once

#include "..//Time/DayCounter.h"

namespace JSLib {

	enum Compounding {
		Simple = 0,
		Compounded = 1,
		Continuous = 2,
	};

	enum Frequency {
		NoFrequency = -1,
		Once = 0,
		Annual = 1,
		Semiannual = 2,
		Quarterly = 4,
		Monthly = 12,
		Weekly = 52,
		Daily = 365,
	};

	class InterestRate {
	public:
		InterestRate();
		InterestRate(double r, const DayCounter& dc, Compounding comp, Frequency freq);
		
		operator double() const { return r_; }
		double rate() const { return r_; }
		const DayCounter& dayCounter() const { return dc_; }
		Compounding compounding() const { return comp_; }
		Frequency frequency() const { return freqMakesSense_ ? Frequency(int(freq_)) : NoFrequency; }

		double discountFactor(double t) const;
		double discountFactor(const Date& d1, const Date& d2) const;
		double compoundFactor(double t) const;	// t : year fraction
		double compoundFactor(const Date& d1, const Date& d2) const;

		// 왜 static? 그냥 밖으로 빼면 안되나?
		static InterestRate impliedRate(double compound, // compound : growth factor 
			const DayCounter&, Compounding, Frequency, double t);
		static InterestRate impliedRate(double compound,
			const DayCounter&, Compounding, Frequency,
			const Date& d1, const Date& d2);

	private:
		double r_;
		DayCounter dc_;
		Compounding comp_;
		bool freqMakesSense_;
		double freq_;
	};



}