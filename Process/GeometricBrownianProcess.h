#pragma once

#include <memory>

#include "StochasticProcess.h"
#include "EulerDiscretization.h"

namespace JSLib{

	class GeometricBrownianMotionProcess : public StochasticProcess1D {
	public:
		GeometricBrownianMotionProcess(double initialValue,
			double mue, double sigma);
		double x0() const;
		double drift(double t, double x) const;
		double diffusion(double t, double x) const;
	protected:
		double initialValue_;
		double mue_;
		double sigma_;
	};

}