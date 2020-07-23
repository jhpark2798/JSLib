#pragma once
#include "Utility.h"
#include "DesignPattern/Visitor.h"
#include "Option.h"

#include <algorithm>

namespace JSLib {

	class Payoff {
	public:
		Payoff() {}
		virtual ~Payoff() {}
		virtual double operator()(double price) const = 0;
		virtual void accept(AcyclicVisitor&);
	};

	// intermediate class for put/call payoff
	class TypePayoff : public Payoff {
	public:
		Option::Type optionType() const { return type_; }
	protected:
		explicit TypePayoff(Option::Type type) : type_(type) {}
		Option::Type type_;
	};

	// intermediate class for payoff based on fixed strike 
	class StrikedTypePayoff : public TypePayoff {
	public:
		double strike() const { return strike_; }
	protected:
		explicit StrikedTypePayoff(Option::Type type, double strike) 
			: TypePayoff(type), strike_(strike) {}
		double strike_;
	};

	class PlainVanillaPayoff : public StrikedTypePayoff {
	public:
		PlainVanillaPayoff(Option::Type type, double strike)
			: StrikedTypePayoff(type, strike) {}
		double operator()(double price) const {
			return std::max(static_cast<int>(type_) * (price - strike_), 0.0);
		}
		virtual void accept(AcyclicVisitor&);
	};

	class CashOrNothingPayoff : public StrikedTypePayoff {
	public:
		CashOrNothingPayoff(Option::Type type, double strike, double cashPayoff) 
			: StrikedTypePayoff(type, strike), cashPayoff_(cashPayoff) {}
		double cashPayoff() const { return cashPayoff_; }
		double operator()(double price) const {
			return cashPayoff_ *
				static_cast<int>(type_) * static_cast<int>((price - strike_) > 0);
		}
		virtual void accept(AcyclicVisitor&);
	private:
		double cashPayoff_;
	};

	inline void Payoff::accept(AcyclicVisitor& v) {
		auto v1 = dynamic_cast<Visitor<Payoff>*>(&v);
		if (v1 != nullptr)
			v1->visit(*this);
		else {
			JS_REQUIRE(false, "not a payoff visitor");
		}
	}

	inline void PlainVanillaPayoff::accept(AcyclicVisitor& v) {
		auto v1 = dynamic_cast<Visitor<PlainVanillaPayoff>*>(&v);
		if (v1 != nullptr)
			v1->visit(*this);
		else {
			JS_FAIL("not a plain vanilla payoff visitor");
		}
	}

	inline void CashOrNothingPayoff::accept(AcyclicVisitor& v) {
		auto v1 = dynamic_cast<Visitor<CashOrNothingPayoff>*>(&v);
		if (v1 != nullptr)
			v1->visit(*this);
		else {
			JS_FAIL("not a cash or nothing payoff visitor");
		}
	}

}