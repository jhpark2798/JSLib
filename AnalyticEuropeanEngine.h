#pragma once

#include "VanillaOption.h"
#include "Process/GeometricBrownianProcess.h"
#include "Process/StochasticProcess.h"
#include "Utility.h"

namespace JSLib {

	class AnalyticEuropeanEngine
		: public VanillaOption::engine {
	public:
		AnalyticEuropeanEngine(const std::shared_ptr<StochasticProcess1D>& process)
			: process_(process) {}
		void calculate() const;
	private:
		std::shared_ptr<StochasticProcess1D> process_;
	};

}
