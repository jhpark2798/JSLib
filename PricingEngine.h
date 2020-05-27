#pragma once
#include "DesignPattern/Observer.h"

namespace JSLib {
	
	class PricingEngine : public Observable {
	public:
		class arguments;
		class results;

		PricingEngine() {}
		virtual ~PricingEngine() {}

		virtual arguments* getArguments() const = 0;
		virtual const results* getResults() const = 0;
		virtual void reset() const = 0;
		virtual void calculate() const = 0;
	};

	class PricingEngine::arguments {
	public:
		arguments() {}
		virtual ~arguments() {}
		virtual void validate() const = 0;
	};

	class PricingEngine::results {
	public:
		results() {}
		virtual ~results() {}
		virtual void reset() = 0;
	};

	template <class ArgumentsType, class ResultsType>
	class GenericEngine : public PricingEngine,  public Observer {
	public:
		GenericEngine() {}
		// pointer¸¦ ¸®ÅÏÇÒ°Å¸é Ä¸½¶È­´Â ¹¹ÇÏ·¯ ÇßÁö?
		PricingEngine::arguments* getArguments() const {
			return &arguments_;
		}
		PricingEngine::results* getResults() const {
			return &results_;
		}
		void reset() const { results_.reset(); }
		void update() { notifyObservers(); }
	protected:
		mutable ArgumentsType arguments_;
		mutable ResultsType results_;
	};

}

