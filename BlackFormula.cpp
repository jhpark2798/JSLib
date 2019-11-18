#include "BlackFormula.h"
#include "Utility.h"
#include <boost/math/distributions/normal.hpp>
#include <cmath>

namespace {
	double nd1(double s, double r, double sigma, double tau, double K) {
		double d1 = (std::log(s / K) + (r + 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
		boost::math::normal norm;
		return boost::math::cdf(norm, d1);
	}

	double nd2(double s, double r, double sigma, double tau, double K) {
		double d2 = (std::log(s / K) + (r - 0.5 * sigma * sigma) * tau) / (sigma * std::sqrt(tau));
		boost::math::normal norm;
		return boost::math::cdf(norm, d2);
	}
}

namespace JSLib {

	double blackFormula(OptionType type,
		double s, double r, double sigma, double tau, double K) {
		return type * (s * nd1(s, r, sigma, tau, K) 
			- K * std::exp(-r * tau) * nd2(s, r, sigma, tau, K));
	}

}