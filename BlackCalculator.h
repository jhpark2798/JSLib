#pragma once
#include "Payoff.h"
#include "Time/Date.h"
#include <memory>

namespace JSLib {

	class BlackCalculator {
	public:
		// s, r, tau, sigma, K, q, optionType ÇÊ¿ä
		// payoff => K, optionType
		BlackCalculator(const std::shared_ptr<StrikedTypePayoff> payoff,
			double spot, double vol, double riskLessRate, Date maturity, double div);
		
		double value() const;
		double delta() const;
		double gamma() const;
		double theta() const;
		double vega() const;
		double rho() const;

	private:
		double strike_, spot_, vol_, riskLessRate_, div_;
		OptionType type_;
		Date maturity_;

		double TTM_;
		double d1_, d2_;
		double nd1_, cum_nd1_, nd2_, cum_nd2_;
		double cum_n_md1_, cum_n_md2_;
		double divDf_, yieldDf_;
	};

}