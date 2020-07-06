#include "OneAssetOption.h"

namespace JSLib {

	OneAssetOption::OneAssetOption(
		const std::shared_ptr<Payoff>& payoff, const std::shared_ptr<Exercise>& exercise)
		: Option(payoff, exercise) {}

	bool OneAssetOption::isExpired() const {
		return exercise_->lastDate() < Settings::instance().evaluationDate();
	}

	void OneAssetOption::fetchResults(const PricingEngine::results* r) const {
		Option::fetchResults(r);
		const Greeks* results = dynamic_cast<const Greeks*>(r);
		JS_REQUIRE(r, "no greeks returned from pricing engine");
		delta_ = results->delta;
		gamma_ = results->gamma;
		theta_ = results->theta;
		vega_ = results->vega;
		rho_ = results->rho;
	}

	void OneAssetOption::setupExpired() const {
		Option::setupExpired();
		delta_ = gamma_ = theta_ = vega_ = rho_ = 0;
	}

}