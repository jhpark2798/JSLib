#include <iostream>
#include <memory>
#include "Utility.h"
#include "DesignPattern/Observer.h"
#include "AnalyticEuropeanEngine.h"
#include "Time/Date.h"
#include "Time/Actual365.h"
#include "GBMProcess.h"
#include "Payoff.h"
#include "Exercise.h"
#include "Settings.h"

using std::cout;
using std::endl;
using namespace JSLib;

int main() {
	std::shared_ptr<GBMProcess> process = std::make_shared<GBMProcess>(100, 0, 0.2);
	double riskLessRate = 0.02;
	Date today = Date(2019, 11, 18);
	Date maturDate(2019, 12, 18);
	OptionType optionType = Call;
	double strike = 100;
	Settings::instance().evaluationDate() = today;
	std::shared_ptr<PricingEngine> engine = 
		std::make_shared<AnalyticEuropeanEngine>(process, riskLessRate);
	std::shared_ptr<Payoff> payoff = std::make_shared<PlainVanillaPayoff>(optionType, strike);
	std::shared_ptr<Exercise> exercise = std::make_shared<EuropeanExercise>(maturDate);
	VanillaOption option(payoff, exercise);
	option.setPricingEngine(engine);

	cout << "price : " << option.NPV() << endl;
	cout << "delta : " << option.delta() << endl;
	cout << "gamma : " << option.gamma() << endl;
	cout << "theta : " << option.theta() << endl;
	cout << "vega : " << option.vega() << endl;
	cout << "rho : " << option.rho() << endl;

	Actual365 dayCounter{};
	cout << dayCounter.yearFraction(Date(2019, 11, 1), Date(2019, 11, 10)) << endl;

	return 0;
}