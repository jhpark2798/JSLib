#include <iostream>
#include <memory>
#include <vector>

#include "Utility.h"
#include "DesignPattern/Observer.h"
#include "AnalyticEuropeanEngine.h"
#include "Time/Date.h"
#include "Time/Actual365.h"
#include "GBMProcess.h"
#include "Payoff.h"
#include "Exercise.h"
#include "Settings.h"
#include "Math/LinearInterpolation.h"
#include "TermStructure/InterpolatedZeroCurve.h"

using std::cout;
using std::endl;
using namespace JSLib;

int main() {

	// plain vanilla call option pricing example
	//std::shared_ptr<GBMProcess> process = std::make_shared<GBMProcess>(100, 0, 0.2);
	//double riskLessRate = 0.02;
	//Date today = Date(2019, 11, 18);
	//Date maturDate(2019, 12, 18);
	//OptionType optionType = Call;
	//double strike = 100;
	//Settings::instance().evaluationDate() = today;
	//std::shared_ptr<PricingEngine> engine = 
	//	std::make_shared<AnalyticEuropeanEngine>(process, riskLessRate);
	//std::shared_ptr<Payoff> payoff = std::make_shared<PlainVanillaPayoff>(optionType, strike);
	//std::shared_ptr<Exercise> exercise = std::make_shared<EuropeanExercise>(maturDate);
	//VanillaOption option(payoff, exercise);
	//option.setPricingEngine(engine);

	//cout << "price : " << option.NPV() << endl;
	//cout << "delta : " << option.delta() << endl;
	//cout << "gamma : " << option.gamma() << endl;
	//cout << "theta : " << option.theta() << endl;
	//cout << "vega : " << option.vega() << endl;
	//cout << "rho : " << option.rho() << endl;
	//cout << endl;

	// Day count convention example
	//Actual365 dayCounter{};
	//cout << dayCounter.yearFraction(Date(2019, 11, 1), Date(2019, 11, 10)) << endl;
	//cout << endl;
	
	// Linear interpolation example
	//std::vector<double> dates = { 0.1, 0.2, 0.3, 0.5 };
	//std::vector<double> rates = { 0.015, 0.02, 0.022, 0.025 };
	//LinearInterpolation interpolation(dates.begin(), dates.end(), rates.begin());
	//cout << interpolation(0.4) << endl;
	//cout << interpolation(0.51) << endl;	// error. interpolation does not allow extrapolation
	//cout << endl;

	// InterpolatedZeroCurve example
	std::vector<Date> dates = { Date(2020,1,30), Date(2020,2,30),
	Date(2020, 4, 15), Date(2020,8,10) };
	std::vector<double> yields = { 0.02, 0.03, 0.032, 0.04 };
	// object slicing은 위험하다고 알고 있는데 이런 코드를 만드는게 잘하는 일인가?
	InterpolatedZeroCurve<Linear> yieldStructure(dates, yields, Actual365());
	cout << yieldStructure.zeroRate(Date(2020, 3, 30), Actual365(), Continuous) << endl;
	return 0;

}



