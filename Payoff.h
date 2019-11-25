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

	// intermediate class for put/call payoff
	class TypePayoff : public Payoff {
	public:
		OptionType optionType() const { return type_; }
	protected:
		explicit TypePayoff(OptionType type) : type_(type) {}
		OptionType type_;
	};

	// intermediate class for payoff based on fixed strike 
	class StrikedTypePayoff : public TypePayoff {
	public:
		double strike() const { return strike_; }
	protected:
		explicit StrikedTypePayoff(OptionType type, double strike) 
			: TypePayoff(type), strike_(strike) {}
		double strike_;
	};

	class PlainVanillaPayoff : public StrikedTypePayoff {
	public:
		PlainVanillaPayoff(OptionType type, double strike)
			: StrikedTypePayoff(type, strike) {}
		double operator()(double price) const;
	};

}