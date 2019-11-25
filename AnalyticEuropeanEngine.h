#pragma once
#include "VanillaOption.h"
#include "GBMProcess.h"
#include "Utility.h"

namespace JSLib {

	class AnalyticEuropeanEngine
		: public VanillaOption::engine {
	public:
		AnalyticEuropeanEngine(const std::shared_ptr<GBMProcess>& process,
			double riskLessRate)
			: process_(process), r_(riskLessRate) {}
		void calculate() const;
	private:
		std::shared_ptr<GBMProcess> process_;
		double r_;
	};

}
