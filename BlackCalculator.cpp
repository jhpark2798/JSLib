#include "BlackCalculator.h"
#include "Settings.h"
#include <boost/math/distributions/normal.hpp>
#include <cmath>

namespace JSLib {

	BlackCalculator::BlackCalculator(
		const std::shared_ptr<StrikedTypePayoff> payoff, double spot,
		double vol, double riskLessRate, Date maturity, double div)
		: strike_(payoff->strike()), type_(payoff->optionType()), spot_(spot),
		vol_(vol), riskLessRate_(riskLessRate), maturity_(maturity), div_(div) {
		using boost::math::cdf;
		using boost::math::pdf;
		TTM_ = daysBetween(Settings::instance().evaluationDate(), maturity) / DAY_OF_YEAR;
		d1_ = (std::log(spot_ / strike_) 
			+ (riskLessRate_ - div_ + 0.5 * vol_ * vol_) * TTM_) / (vol_ * std::sqrt(TTM_));
		d2_ = d1_ - vol_ * std::sqrt(TTM_);
		boost::math::normal norm;
		nd1_ = pdf(norm, d1_);
		cum_nd1_ = cdf(norm, d1_);
		cum_n_md1_ = 1 - cum_nd1_;
		nd2_ = pdf(norm, d2_);
		cum_nd2_ = cdf(norm, d2_);
		cum_n_md2_ = 1 - cum_nd2_;
		divDf_ = std::exp(-div_ * TTM_);
		yieldDf_ = std::exp(-riskLessRate_ * TTM_);
	}

	double BlackCalculator::value() const {
		switch (type_) {
		case Call:
			return spot_ * divDf_ * cum_nd1_ - strike_ * yieldDf_ * cum_nd2_;
		case Put:
			return strike_ * yieldDf_ * cum_n_md2_ - spot_ * divDf_ * cum_n_md1_;
		}
	}

	double BlackCalculator::delta() const {
		switch (type_) {
		case Call:
			return divDf_ * cum_nd1_;
		case Put:
			return -divDf_ * cum_n_md1_;
		}
	}

	double BlackCalculator::gamma() const {
		return strike_ * yieldDf_ * nd2_ / (spot_ * spot_ * vol_ * std::sqrt(TTM_));
	}

	double BlackCalculator::theta() const {
		switch (type_) {
		case Call:
			return -divDf_ * spot_ * nd1_ * vol_ / 2 / std::sqrt(TTM_)
				- riskLessRate_ * strike_ * yieldDf_ * cum_nd2_
				+ div_ * spot_ * divDf_ * cum_nd1_;
		case Put:
			return -divDf_ * spot_ * nd1_ * vol_ / 2 / std::sqrt(TTM_)
				+ riskLessRate_ * strike_ * yieldDf_ * cum_n_md2_
				- div_ * spot_ * divDf_ * cum_n_md1_;
		}
	}

	double BlackCalculator::vega() const {
		return strike_ * yieldDf_ * nd2_ * std::sqrt(TTM_);
	}

	double BlackCalculator::rho() const {
		switch (type_) {
		case Call:
			return strike_ * TTM_ * yieldDf_ * cum_nd2_;
		case Put:
			return -strike_ * TTM_ * yieldDf_ * cum_n_md2_;
		}
	}

}