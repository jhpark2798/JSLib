#pragma once
#include "Instrument.h"
#include "PricingEngine.h"

#include <string>
#include <memory>

namespace JSLib {

	class Payoff;
	class Exercise;

	class Option : public Instrument {
	public:
		class arguments;
		enum Type { Put = -1, Call = 1 };
		Option(const std::shared_ptr<Payoff>& payoff, 
			const std::shared_ptr<Exercise>& exercise)
			: payoff_(payoff), exercise_(exercise) {}
		void setupArguments(PricingEngine::arguments* args) const override;
		std::shared_ptr<Payoff> payoff() { return payoff_; }
		std::shared_ptr<Exercise> exercise() { return exercise_; }
	protected:
		std::shared_ptr<Payoff> payoff_;
		std::shared_ptr<Exercise> exercise_;
	};

	std::ostream& operator<< (std::ostream&, Option::Type);

	class Option::arguments : public virtual PricingEngine::arguments {
	public:
		arguments() {}
		void validate() const {
			JS_REQUIRE(payoff, "no payoff given");
			JS_REQUIRE(exercise, "no exercise given");
		}
		std::shared_ptr<Payoff> payoff;
		std::shared_ptr<Exercise> exercise;
	};

	// Greeks 클래스는 OneAssetOption에 있어야 한다고 생각해서 뺐음 
	// asset 개수에 따라 greek 종류가 다르니까

	inline void Option::setupArguments(PricingEngine::arguments* args) const {
		Option::arguments* arguments = dynamic_cast<Option::arguments*>(args);
		JS_REQUIRE(arguments, "wrong argument type");
		arguments->payoff = payoff_;
		arguments->exercise = exercise_;
	}

	inline std::ostream& operator<<(std::ostream& out, Option::Type type) {
		switch (type) {
		case Option::Call:
			return out << std::string("Call");
		case Option::Put:
			return out << std::string("Put");
		default:
			JS_REQUIRE(false, "unknown option type");
		}
	}
}