#include "AnalyticEuropeanEngine.h"
#include "Exercise.h"
#include "Utility.h"
#include "Date.h"
#include "BlackFormula.h"
#include "Settings.h"
#include <memory>

namespace JSLib {

	void AnalyticEuropeanEngine::calculate() const {
		JS_REQUIRE(arguments_.exercise->type() == Exercise::European,
			"not an European option");
		std::shared_ptr<PlainVanillaPayoff> payoff =
			std::dynamic_pointer_cast<PlainVanillaPayoff>(arguments_.payoff);
		JS_REQUIRE(payoff, "not an PlainVanillaPayoff");
		JS_REQUIRE(process_, "GBM Process needed");
		Date evalDate = Settings::instance().evaluationDate();
		double spot = process_->x0();
		double sigma = process_->diffusion(1);
		double tau = daysBetween(evalDate, arguments_.exercise->lastDate()) / DAY_OF_YEAR;
		double r = r_;
		double K = arguments_.payoff->strike();
		results_.value = blackFormula(type_, spot, r, sigma, tau, K);
		// greek 계산 코드 추가 필요
	}

}