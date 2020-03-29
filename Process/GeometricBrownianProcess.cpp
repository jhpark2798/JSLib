#include "GeometricBrownianProcess.h"

namespace JSLib {

	GeometricBrownianMotionProcess::GeometricBrownianMotionProcess(
		double initialValue, double mue, double sigma)
		: StochasticProcess1D(std::shared_ptr<discretization>(new EulerDiscretization)),
		initialValue_(initialValue), mue_(mue), sigma_(sigma) {}

	double GeometricBrownianMotionProcess::x0() const {
		return initialValue_;
	}

	double GeometricBrownianMotionProcess::drift(double, double x) const {
		return mue_ * x;
	}

	double GeometricBrownianMotionProcess::diffusion(double, double x) const {
		return sigma_ * x;
	}

}