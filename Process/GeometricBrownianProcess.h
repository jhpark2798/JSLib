#pragma once

#include <memory>

#include "StochasticProcess.h"
#include "EulerDiscretization.h"
#include "..//TermStructure/YieldTermStructure.h"

namespace JSLib{

	class GeometricBrownianMotionProcess : public StochasticProcess1D {
	public:
		GeometricBrownianMotionProcess(double initialValue,
			double mue, double sigma, const std::shared_ptr<YieldTermStructure>&);
		double x0() const;
		double drift(double t, double x) const;
		double diffusion(double t, double x) const;
		double time(const Date& d) const;
		const std::shared_ptr<YieldTermStructure>& riskFreeRate() const {
			return riskFreeRate_;
		}

	protected:
		double initialValue_;
		double mue_;
		double sigma_;
		std::shared_ptr<YieldTermStructure> riskFreeRate_;
	};

}