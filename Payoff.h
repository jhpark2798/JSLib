#pragma once
#include "Utility.h"

namespace JSLib {

	class Payoff {
	public:
		Payoff() {}
		virtual ~Payoff() {}
		virtual double operator()(double price) const = 0;
		virtual double strike() const = 0;
	};

	class PlainVanillaPayoff : public Payoff {
	public:
		PlainVanillaPayoff(double strike, OptionType type) :
			strike_(strike), type_(type) {}
		double operator()(double price) const;
		double strike() const { return strike_; }
	private:
		double strike_;
		OptionType type_;
	};

}