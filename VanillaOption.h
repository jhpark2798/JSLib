#pragma once
#include "Instrument.h"
#include "Payoff.h"
#include "Exercise.h"
#include "Date.h"
#include <memory>

namespace JSLib {

	class VanillaOption : public Instrument {
	public:
		class arguments;
		class results;
		class engine;
		VanillaOption(const std::shared_ptr<Payoff>&,
			const std::shared_ptr<Exercise>&);

		double errorEstimate() const;
		bool isExpired() const;
		void setupArguments(PricingEngine::arguments* a) const;
		void fetchResults(PricingEngine::results* r) const;

		double delta() const;
		double gamma() const;
		double theta() const;
		double vega() const;
		double rho() const;

	protected:
		void setupExpired() const;

		std::shared_ptr<Payoff> payoff_;
		std::shared_ptr<Exercise> exercise_;
		
		mutable double delta_;
		mutable double gamma_;
		mutable double theta_;
		mutable double vega_;
		mutable double rho_;
	};

	class VanillaOption::arguments : public PricingEngine::arguments {
	public:
		arguments() {}
		void validate() const;
		std::shared_ptr<Payoff> payoff;
		std::shared_ptr<Exercise> exercise;
	};

	class Greeks : public virtual PricingEngine::results {
	public:
		Greeks() {}
		void reset();
		double delta, gamma;
		double theta;
		double vega;
		double rho;
	};

	class VanillaOption::results : public Instrument::results, public Greeks {
	public:
		results() {}
		void reset();
	};

	class VanillaOption::engine
		: public GenericEngine<VanillaOption::arguments,
		VanillaOption::results> {};

}