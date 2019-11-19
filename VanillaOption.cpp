#include "VanillaOption.h"
#include "Utility.h"
#include "Settings.h"
#include <stdexcept>

namespace JSLib {

	VanillaOption::VanillaOption(const std::shared_ptr<Payoff>& payoff,
		const std::shared_ptr<Exercise>& exercise) 
		: payoff_(payoff), exercise_(exercise) {}

	double VanillaOption::errorEstimate() const {
		calculate();
		if (errorEstimate_ == NULL_DOUBLE) 
			throw std::runtime_error("error estimate is not provided");
		return errorEstimate_;
	}

	bool VanillaOption::isExpired() const {
		return exercise_->lastDate() < Settings::instance().evaluationDate();
	}

	void VanillaOption::setupExpired() const {
		Instrument::setupExpired();
		delta_ = gamma_ = theta_ = vega_ = rho_ = 0.0;
	}

	void VanillaOption::setupArguments(PricingEngine::arguments* args) const {
		VanillaOption::arguments* arguments =
			dynamic_cast<VanillaOption::arguments*>(args);
		JS_REQUIRE(arguments != nullptr, "wrong argument type");
		arguments->exercise = exercise_;
		arguments->payoff = payoff_;
	}

	void VanillaOption::fetchResults(PricingEngine::results* r) const {
		VanillaOption::results* results = dynamic_cast<VanillaOption::results*>(r);
		JS_REQUIRE(results, "wrong results type");
		delta_ = results->delta;
		gamma_ = results->gamma;
		theta_ = results->theta;
		vega_ = results->vega;
		rho_ = results->rho;
	}

	double VanillaOption::delta() const {
		calculate();
		JS_REQUIRE(delta_ != NULL_DOUBLE, "delta is not provided");
		//if (delta_ == NULL_DOUBLE)
		//	throw std::runtime_error("delta is not provided");
		return delta_;
	}

	double VanillaOption::gamma() const {
		calculate();
		JS_REQUIRE(gamma_ != NULL_DOUBLE, "gamma is not provided");
		return gamma_;
	}

	double VanillaOption::theta() const {
		calculate();
		JS_REQUIRE(theta_ != NULL_DOUBLE, "theta is not provided");
		return theta_;
	}

	double VanillaOption::vega() const {
		calculate();
		JS_REQUIRE(vega_ != NULL_DOUBLE, "vega is not provided");
		return vega_;
	}

	double VanillaOption::rho() const {
		calculate();
		JS_REQUIRE(rho_ != NULL_DOUBLE, "rho is not provided");
		return rho_;
	}

	void VanillaOption::arguments::validate() const {
		JS_REQUIRE(payoff, "no payoff given");
		JS_REQUIRE(exercise, "no payoff given");
	}

	void Greeks::reset() {
		delta = gamma = theta = vega = rho = NULL_DOUBLE;
	}

	void VanillaOption::results::reset() {
		Instrument::results::reset();
		Greeks::reset();
	}

}