#pragma once

#include "StochasticProcess.h"
#include "..//Quote.h"
#include "..//TermStructure/YieldTermStructure.h"
#include "..//TermStructure/BlackVolTermStructure.h"
#include "EulerDiscretization.h"

#include <memory>

namespace JSLib {

	class GeneralizedBlackScholesProcess : public StochasticProcess1D {
	public:
		GeneralizedBlackScholesProcess(
			const std::shared_ptr<Quote>& x0,
			const std::shared_ptr<YieldTermStructure>& dividendTS,
			const std::shared_ptr<YieldTermStructure>& riskFreeTS,
			const std::shared_ptr<BlackVolTermStructure>& blackVolTS,
			const std::shared_ptr<discretization>& d = std::make_shared<EulerDiscretization>());
		double x0() const {
			return x0_->value();
		}
		double drift(double t, double x) const;
		double diffusion(double t, double x) const;
		double apply(double x0, double dx) const {
			return x0 * std::exp(dx);
		}
		double expectation(double t0, double x0, double dt) const {
			JS_REQUIRE("false", "not implemented");
		}
		double evolve(double t0, double x0, double dt, double dw) const;
		double time(const Date& d) const;
		const std::shared_ptr<Quote>& stateVariable() const {
			return x0_;
		}
		const std::shared_ptr<YieldTermStructure>& dividendYield() const {
			return dividendYield_;
		}
		const std::shared_ptr<YieldTermStructure>& riskFreeRate() const {
			return riskFreeRate_;
		}
		const std::shared_ptr<BlackVolTermStructure>& blackVolatility() const {
			return blackVolatility_;
		}
	private:
		std::shared_ptr<Quote> x0_;
		std::shared_ptr<YieldTermStructure>riskFreeRate_, dividendYield_;
		std::shared_ptr<BlackVolTermStructure> blackVolatility_;
	};

}