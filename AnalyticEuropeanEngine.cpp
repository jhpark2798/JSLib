#include "AnalyticEuropeanEngine.h"
#include "Exercise.h"
#include "Utility.h"
#include "Time/Date.h"
#include "BlackFormula.h"
#include "Settings.h"
#include "BlackCalculator.h"
#include <memory>

namespace JSLib {

	void AnalyticEuropeanEngine::calculate() const {
		JS_REQUIRE(arguments_.exercise->type() == Exercise::European,
			"not an European option");
		std::shared_ptr<PlainVanillaPayoff> payoff =
			std::dynamic_pointer_cast<PlainVanillaPayoff>(arguments_.payoff);
		JS_REQUIRE(payoff, "not an PlainVanillaPayoff");
		JS_REQUIRE(process_, "GBM Process needed");
		OptionType type = payoff->optionType();
		Date evalDate = Settings::instance().evaluationDate();
		double spot = process_->x0();
		double sigma = process_->diffusion(1);
		double r = r_;
		double K = arguments_.payoff->strike();
		// results_.value = blackFormula(type, spot, r, sigma, tau, K);
		// greek 계산 코드 추가 필요
		BlackCalculator black(payoff, spot, sigma, r, arguments_.exercise->lastDate(), 0);
		results_.value = black.value();
		results_.delta = black.delta();
		results_.gamma = black.gamma();
		results_.theta = black.theta();
		results_.vega = black.vega();
		results_.rho = black.rho();
		results_.errorEstimate = 0;
	}

}