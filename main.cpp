#include <iostream>
#include <memory>
#include "Utility.h"
#include "Observer.h"
#include "AnalyticEuropeanEngine.h"
#include "Date.h"
#include "GBMProcess.h"
#include "Payoff.h"
#include "Exercise.h"

using std::cout;
using std::endl;
using namespace JSLib;

int main() {
	std::shared_ptr<GBMProcess> process = std::make_shared<GBMProcess>(100, 0, 0.2);
	double riskLessRate = 0.02;
	Date evalDate = Date(2019, 11, 18);
	Date maturDate = Date(2019, 12, 18);
	OptionType optionType = Call;
	double strike = 100;
	std::shared_ptr<PricingEngine> engine = 
		std::make_shared<AnalyticEuropeanEngine>(process, riskLessRate, evalDate, optionType);
	std::shared_ptr<Payoff> payoff = std::make_shared<PlainVanillaPayoff>(strike, optionType);
	std::shared_ptr<Exercise> exercise = std::make_shared<EuropeanExercise>(maturDate);
	VanillaOption option(payoff, exercise);
	option.setPricingEngine(engine);
	option.setEvalDate(evalDate);
	cout << option.NPV() << endl;
	return 0;
}