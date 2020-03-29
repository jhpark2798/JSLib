#include "Eulerdiscretization.h"

namespace JSLib {

	Disposable<Array> EulerDiscretization::drift(
		const StochasticProcess& process, double t0, 
		const Array& x0, double dt) const {
		return process.drift(t0, x0) * dt;
	}

	double EulerDiscretization::drift(const StochasticProcess1D& process, 
		double t0, double x0, double dt) const {
		return process.drift(t0, x0) * dt;
	}

	Disposable<Matrix> EulerDiscretization::diffusion(
		const StochasticProcess& process, double t0, 
		const Array& x0, double dt) const {
		return process.diffusion(t0, x0) * std::sqrt(dt);
	}

	double EulerDiscretization::diffusion(
		const StochasticProcess1D& process, double t0, 
		double x0, double dt) const {
		return process.diffusion(t0, x0) * std::sqrt(dt);
	}

	Disposable<Matrix> EulerDiscretization::covariance(
		const StochasticProcess& process, double t0, 
		const Array& x0, double dt) const {
		Matrix sigma = process.diffusion(t0, x0);
		Matrix result = sigma * transpose(sigma) * dt;
		return result;
	}

	double EulerDiscretization::variance(
		const StochasticProcess1D& process, double t0, 
		double x0, double dt) const {
		double sigma = process.diffusion(t0, x0);
		return sigma * sigma * dt;
	}

}