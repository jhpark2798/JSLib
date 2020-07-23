#pragma once
#include "Option.h"
#include "Payoff.h"
#include "Time/Date.h"
#include "DesignPattern/Visitor.h"

#include <memory>

namespace JSLib {

	class BlackCalculator {
	private:
		class Calculator;
		friend class Calculator;
	public:
		BlackCalculator(const std::shared_ptr<StrikedTypePayoff>& payoff,
			double spot, double forward, double stdDev, double discount = 1.0);
		BlackCalculator(Option::Type optionType,
			double spot, double strike, double forward, double stdDev, double discount = 1.0);
		virtual ~BlackCalculator() {}		// BlackCalculator의 파생 클래스가 있나봄
		
		double value() const;
		virtual double delta() const;
		virtual double gamma() const;
		virtual double theta() const;
		double vega() const;	// 왜 안 virtual?
		double rho() const;	// 왜 안 virtual?

		//virtual double delta(double spot) const;
		//virtual double gamma(double spot) const;
		//virtual double theta(double spot, double maturity) const;
		//double vega(double maturity) const;	// 왜 안 virtual?
		//double rho(double maturity) const;	// 왜 안 virtual?

	protected:
		void initialize(const std::shared_ptr<StrikedTypePayoff>& p);
		double spot_, strike_, forward_, stdDev_, discount_, variance_;
		double d1_, d2_;
		double n_d1_, cum_d1_, n_d2_, cum_d2_;

		double value_;
		double delta_, gamma_, theta_, vega_, rho_;
	};

}