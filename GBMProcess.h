#pragma once

namespace JSLib{

	class GBMProcess {
	public:
		GBMProcess(double initialValue, double mu, double sigma)
			: initialValue_(initialValue), mu_(mu), sigma_(sigma) {}
		double x0() const { return initialValue_; }
		double drift(double s) const { return mu_ * s; }
		double diffusion(double s) const { return sigma_ * s; }
	private:
		double initialValue_;
		double mu_;
		double sigma_;
	};

}