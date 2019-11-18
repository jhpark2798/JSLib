#pragma once

#include "LazyObject.h"
#include "PricingEngine.h"
#include "Utility.h"
#include <string>
#include <memory>

namespace JSLib {

	class Instrument : public LazyObject {
	public:
		class results;
		Instrument() {}
		~Instrument() {}

		virtual double NPV() const;
		virtual double errorEstimate() const = 0;
		virtual bool isExpired() const = 0;
		void calculate() const;
		virtual void setupExpired() const;
		virtual void performCalculations() const;
		virtual void setupArguments(PricingEngine::arguments* a) const;
		virtual void fetchResults(const PricingEngine::results* r) const;
		void setPricingEngine(const std::shared_ptr<PricingEngine>& e);
		//	value, errorEstimate 외에 additionalResult가 있을 때 필요한 함수
		// template <class T> T result(const std::string& tag) const;
	protected:
		mutable double NPV_;
		mutable double errorEstimate_;
		std::shared_ptr<PricingEngine> engine_;
	};

	class Instrument::results : public virtual PricingEngine::results {
	public:
		results() {}
		void reset() { value = errorEstimate = NULL_DOUBLE; }
		double value;	//  왜 캡슐화 안함?
		double errorEstimate;
	};

}