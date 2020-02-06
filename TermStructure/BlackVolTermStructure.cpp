#include "BlackVolTermStructure.h"

namespace JSLib {

	double BlackVolTermStructure::blackVol(const Date& maturity,
		double strike, bool extrapolate) const {
		checkRange(maturity, extrapolate);
		checkStrike(strike, extrapolate);
		double t = timeFromReference(maturity);
		return blackVolImpl(t, strike);
	}

	double BlackVolTermStructure::blackVol(double maturity,
		double strike, bool extrapolate) const {
		checkRange(maturity, extrapolate);
		checkStrike(strike, extrapolate);
		return blackVolImpl(maturity, strike);
	}

	double BlackVolTermStructure::blackVariance(const Date& maturity,
		double strike, bool extrapolate = false) const {
		checkRange(maturity, extrapolate);
		checkStrike(strike, extrapolate);
		double t = timeFromReference(maturity);
		return blackVarianceImpl(t, strike);
	}

	double BlackVolTermStructure::blackVariance(double maturity,
		double strike, bool extrapolate = false) const {
		checkRange(maturity, extrapolate);
		checkStrike(strike, extrapolate);
		return blackVarianceImpl(maturity, strike);
	}



}