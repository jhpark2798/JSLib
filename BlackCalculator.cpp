#include "BlackCalculator.h"
#include "Settings.h"
#include "Utility.h"

#include <boost/math/distributions/normal.hpp>
#include <cmath>

namespace JSLib {

	class BlackCalculator::Calculator : public AcyclicVisitor,
																	public Visitor<Payoff>,
																	public Visitor<PlainVanillaPayoff>,
																	public Visitor<CashOrNothingPayoff> {
	public:
		explicit Calculator(BlackCalculator& black) : black_(black) {}
		void visit(Payoff&);
		void visit(PlainVanillaPayoff&);
		void visit(CashOrNothingPayoff&);
	private:
		BlackCalculator& black_;
	};

	void BlackCalculator::Calculator::visit(Payoff& p) {
		JS_FAIL("unsupported payoff type");
	}

	void BlackCalculator::Calculator::visit(PlainVanillaPayoff& p) {
		switch (p.optionType()) {
		case Option::Type::Call:
			black_.value_ = black_.discount_ * black_.forward_ * black_.cum_d1_ -
				black_.discount_ * black_.strike_ * black_.cum_d2_;
			black_.delta_ = black_.discount_ * black_.forward_ / black_.spot_ * black_.cum_d1_;
			break;
		case Option::Type::Put:
			black_.value_ = -black_.discount_ * black_.forward_ * (1.0-black_.cum_d1_) +
				black_.discount_ * black_.strike_ * (1-black_.cum_d2_);
			black_.delta_ = -black_.discount_ * black_.forward_ / black_.spot_ * (1-black_.cum_d1_);
			break;
		}
		black_.gamma_ = black_.strike_ * black_.discount_ * black_.n_d2_
			/ (black_.spot_*black_.spot_*black_.stdDev_);
		black_.theta_ = 0;
		black_.vega_ = 0;
		black_.rho_ = 0;
	}

	void BlackCalculator::Calculator::visit(CashOrNothingPayoff& p) {
		switch (p.optionType()) {
		case Option::Type::Call:
			black_.value_ = black_.discount_ * p.cashPayoff() * black_.cum_d2_;
			black_.delta_ = black_.discount_ * black_.n_d2_ / (black_.spot_ * black_.stdDev_);
			break;
		case Option::Type::Put:
			black_.value_ = black_.discount_ * p.cashPayoff() * (1-black_.cum_d2_);
			black_.delta_ = -black_.discount_ * black_.n_d2_ / (black_.spot_ * black_.stdDev_);
			break;
		}
		black_.gamma_ = 0;
		black_.theta_ = 0;
		black_.vega_ = 0;
		black_.rho_ = 0;
	}

	BlackCalculator::BlackCalculator(const std::shared_ptr<StrikedTypePayoff>& payoff,
		double spot, double forward, double stdDev, double discount) 
	: spot_(spot), strike_(payoff->strike()), forward_(forward), stdDev_(stdDev), 
		discount_(discount), variance_(stdDev * stdDev) {
		initialize(payoff);
	}

	BlackCalculator::BlackCalculator(Option::Type optionType,
		double spot, double strike, double forward, double stdDev, double discount) 
	: spot_(spot), strike_(strike), forward_(forward), stdDev_(stdDev), 
		discount_(discount), variance_(stdDev*stdDev) {
		auto p = std::make_shared<PlainVanillaPayoff>(optionType, strike);
		initialize(p);
	}

	void BlackCalculator::initialize(const std::shared_ptr<StrikedTypePayoff>& p){
		boost::math::normal norm;
		using boost::math::cdf;
		using boost::math::pdf;
		if (stdDev_ >= JS_EPSILON) {
			if (close(strike_, 0.0)) {

			}
			else {
				d1_ = std::log(forward_ / strike_) / stdDev_ + 0.5 * stdDev_;
				d2_ = d1_ - stdDev_;
				cum_d1_ = cdf(norm, d1_);
				n_d1_ = pdf(norm, d1_);
				cum_d2_ = cdf(norm, d2_);
				n_d2_ = pdf(norm, d2_);
			}
		}
		Calculator calc(*this);
		p->accept(calc);
	}

	double BlackCalculator::value() const {
		return value_;
	}

	double BlackCalculator::delta() const {
		return delta_;
	}

	double BlackCalculator::gamma() const {
		return gamma_;
	}

	double BlackCalculator::theta() const {
		return theta_;
	}

	double BlackCalculator::vega() const {
		return vega_;
	}

	double BlackCalculator::rho() const {
		return rho_;
	}

}