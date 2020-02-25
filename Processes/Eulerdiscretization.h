#pragma once
#include "Stochasticprocess.h"

namespace JSLib {

	class EulerDiscretization : public StochasticProcess::discretization, public StochasticProcess1D::discretization {
	public:
		Disposable<Array> drift(const StochasticProcess&, double t0, const Array& x0, double dt) const;
		double drift(const StochasticProcess1D&, double t0, double x0, double dt) const;

		Disposable<Matrix> diffusion(const StochasticProcess&, double t0, const Array& x0, double dt) const;
		double diffusion(const StochasticProcess1D&, double t0, double x0, double dt) const;

		Disposable<Matrix> covariance(const StochasticProcess&, double t0, const Array& x0, double dt) const;
		double variance(const StochasticProcess1D&, double t0, double x0, double dt) const;
	};

}