#pragma once
#include "VanillaOption.h"
#include "GBMProcess.h"
#include "Utility.h"

namespace JSLib {

	class AnalyticEuropeanEngine
		: public VanillaOption::engine {
	public:
		AnalyticEuropeanEngine(const std::shared_ptr<GBMProcess>& process,
			double riskLessRate, OptionType type)
			: process_(process), r_(riskLessRate), type_(type) {}
		void calculate() const;
	private:
		std::shared_ptr<GBMProcess> process_;
		double r_;
		OptionType type_;
	};

}
