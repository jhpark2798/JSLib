#include "stochasticprocess.h"

namespace JSLib {

	StochasticProcess::StochasticProcess() {}

	StochasticProcess::StochasticProcess(const std::shared_ptr<discretization>& disc) : discretization_(disc) {}

	size_t StochasticProcess::factors() const //바꿔야 할 필요성 -> process를 기술하는 데 몇 차원의 Brownian Motion이 필요한 지를 return해야 함
	{
		return size();
	}

	Disposable<Array> StochasticProcess::expectation(double t0, const Array& x0, double dt) const
	{
		return apply(x0, discretization_->drift(*this, t0, x0, dt));
	}

	Disposable<Matrix> StochasticProcess::stdDeviation(double t0, const Array& x0, double dt) const
	{
		return discretization_->diffusion(*this, t0, x0, dt);
	}

	Disposable<Matrix> StochasticProcess::covariance(double t0, const Array& x0, double dt) const
	{
		return discretization_->covariance(*this, t0, x0, dt);
	}

	Disposable<Array> StochasticProcess::evolve(double t0, const Array& x0, double dt, const Array& dw) const
	{
		return apply(expectation(t0, x0, dt), stdDeviation(t0, x0, dt) * dw);
	}

	Disposable<Array> StochasticProcess::apply(const Array& x0, const Array& dx) const
	{
		return x0 + dx;
	}

	void StochasticProcess::update()
	{
		notifyObservers();
	}

	StochasticProcess1D::StochasticProcess1D() {}

	StochasticProcess1D::StochasticProcess1D(const std::shared_ptr<discretization>& disc) : discretization_(disc) {}

	double StochasticProcess1D::expectation(double t0, double x0, double dt) const 
	{
		return apply(x0, discretization_->drift(*this, t0, x0, dt));
	}

	double StochasticProcess1D::stdDeviation(double t0, double x0, double dt) const 
	{
		return discretization_->diffusion(*this, t0, x0, dt);
	}

	double StochasticProcess1D::variance(double t0, double x0, double dt) const 
	{
		return discretization_->variance(*this, t0, x0, dt);
	}

	double StochasticProcess1D::evolve(double t0, double x0, double dt, double dw) const 
	{
		return apply(expectation(t0, x0, dt), stdDeviation(t0, x0, dt) * dw);
	}

	double StochasticProcess1D::apply(double x0, double dx) const 
	{
		return x0 + dx;
	}

}