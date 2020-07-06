#pragma once

#include "Option.h"
#include "Payoff.h"
#include "Exercise.h"
#include "Null.h"
#include "Settings.h"

#include <memory>

namespace JSLib {

	class OneAssetOption : public Option {
	public:
		class engine;
		class results;
		class Greeks;
		OneAssetOption(const std::shared_ptr<Payoff>& payoff,
			const std::shared_ptr<Exercise>& exercise);
		bool isExpired() const override;
		void fetchResults(const PricingEngine::results* r) const override;
		
		double delta() const { return delta_; }
		double gamma() const { return gamma_; }
		double theta() const { return theta_; }
		double vega() const { return vega_; }
		double rho() const { return rho_; }
	protected:
		void setupExpired() const override;
		mutable double delta_, gamma_, theta_, vega_, rho_;
	};

	class OneAssetOption::Greeks : public virtual PricingEngine::results {
	public:
		void reset() override {
			delta = gamma = theta = vega = rho = dividendRho = Null<double>();
		}
		double delta, gamma, theta, vega, rho, dividendRho;
	};

	class OneAssetOption::results : public Instrument::results,
		public OneAssetOption::Greeks {
	public:
		void reset() override {
			Instrument::results::reset();
			OneAssetOption::Greeks::reset();
		}
	};

	class OneAssetOption::engine :
		public GenericEngine<OneAssetOption::arguments, OneAssetOption::results> 
	{};

}













