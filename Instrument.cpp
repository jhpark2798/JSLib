#include "Instrument.h"
#include <stdexcept>

namespace JSLib {

	double Instrument::NPV() const {
		calculate();
		return NPV_;
	}

	void Instrument::calculate() const {
		if (isExpired()) {
			setupExpired();
			calculated_ = true;
		} else {
			LazyObject::calculate();
		}
	}

	void Instrument::setupExpired() const {
		NPV_ = 0.0;
	}

	void Instrument::performCalculations() const {
		if (!engine_) throw std::runtime_error("null pricing engine");
		engine_->reset();
		setupArguments(engine_->getArguments());
		engine_->getArguments()->validate(); // 왜 validate를 pricing engine에서 안하고 여기서?
		engine_->calculate();
		fetchResults(engine_->getResults());
	}

	void Instrument::setupArguments(PricingEngine::arguments* a) const {	
		// 인자가 왜 const 가 아닐까?
		throw std::runtime_error("setupArguments() not implemented");
	}

	void Instrument::fetchResults(const PricingEngine::results* r) const {
		const Instrument::results* results =
			dynamic_cast<const Instrument::results*>(r);
		if (!results) throw std::runtime_error("no results returned");
		NPV_ = results->value;
		errorEstimate_ = results->errorEstimate;
	}

	void Instrument::setPricingEngine(const std::shared_ptr<PricingEngine>& e) {
		if (engine_) unregisterWith(engine_);
		engine_ = e;
		if (engine_) registerWith(engine_);
		update();
	}

}
