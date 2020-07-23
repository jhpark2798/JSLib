#include "AnalyticEuropeanEngine.h"
#include "Exercise.h"
#include "Utility.h"
#include "Time/Date.h"
#include "Settings.h"
#include "BlackCalculator.h"
#include "Option.h"
#include <memory>

namespace JSLib {

	void AnalyticEuropeanEngine::calculate() const {
		JS_REQUIRE(arguments_.exercise->type() == Exercise::European,
			"not an European option");
		auto payoff = 
			std::dynamic_pointer_cast<StrikedTypePayoff>(arguments_.payoff);
		JS_REQUIRE(payoff, "not an StrikedTypePayoff");
		auto variance = process_->blackVolatility()->blackVariance(
			arguments_.exercise->lastDate(), payoff->strike());
		auto stdDev = std::sqrt(variance);
		auto discount = process_->riskFreeRate()->discount(
			arguments_.exercise->lastDate());
		auto divDiscount = process_->dividendYield()->discount(
			arguments_.exercise->lastDate());
		auto spot = process_->stateVariable()->value();
		auto forward = spot * divDiscount / discount;

		BlackCalculator black(payoff, spot, forward, stdDev, discount);
		results_.value = black.value();
		results_.delta = black.delta();
		results_.gamma = black.gamma();
		results_.theta = black.theta();
		results_.vega = black.vega();
		results_.rho = black.rho();
		results_.errorEstimate = 0;
	}

}

