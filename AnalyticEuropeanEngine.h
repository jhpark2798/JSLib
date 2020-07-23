#pragma once

#include "VanillaOption.h"
#include "Process/GeneralizedBlackScholesProcess.h"
#include "TermStructure/YieldTermStructure.h"
#include "Utility.h"

namespace JSLib {

	class AnalyticEuropeanEngine
		: public VanillaOption::engine {
	public:
		AnalyticEuropeanEngine(
			const std::shared_ptr<GeneralizedBlackScholesProcess>& process)
			: process_(process) {}
		void calculate() const;
	private:
		std::shared_ptr<GeneralizedBlackScholesProcess> process_;
	};

}
