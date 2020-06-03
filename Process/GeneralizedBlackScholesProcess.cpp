#include "GeneralizedBlackScholesProcess.h"

namespace JSLib {

	GeneralizedBlackScholesProcess::GeneralizedBlackScholesProcess(
		const std::shared_ptr<Quote>& x0,
		const std::shared_ptr<YieldTermStructure>& dividendTS,
		const std::shared_ptr<YieldTermStructure>& riskFreeTS,
		const std::shared_ptr<BlackVolTermStructure>& blackVolTS,
		const std::shared_ptr<discretization>& d)
		: StochasticProcess1D(d), x0_(x0), riskFreeRate_(riskFreeTS), 
		dividendYield_(dividendTS), blackVolatility_(blackVolTS) {}

	double GeneralizedBlackScholesProcess::drift(double t, double x) const {
		double sigma = diffusion(t, x);
		return riskFreeRate_->zeroRate(t, Continuous)
			- dividendYield_->zeroRate(t, Continuous)
			- 0.5 * sigma * sigma;
	}

	double GeneralizedBlackScholesProcess::diffusion(double t, double x) const {
		if (t == 0) t += 0.0001;
		return blackVolatility_->blackVol(t, x);
	}

	double GeneralizedBlackScholesProcess::evolve(
		double t0, double x0, double dt, double dw) const {
		double drift = discretization_->drift(*this, t0, x0, dt);
		double diffusion = discretization_->diffusion(*this, t0, x0, dt);
		double res = apply(x0, drift + diffusion * dw);
		return res;
		/*return apply(x0, discretization_->drift(*this, t0, x0, dt) +
			discretization_->diffusion(*this, t0, x0, dt) * dw);*/
	}

	double GeneralizedBlackScholesProcess::time(const Date& d) const {
		return riskFreeRate_->dayCounter().yearFraction(riskFreeRate_->referenceDate(), d);
	}

}