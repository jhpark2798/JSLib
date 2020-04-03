#include <iostream>
#include <memory>
#include <vector>

#include "Utility.h"
#include "DesignPattern/Observer.h"
#include "AnalyticEuropeanEngine.h"
#include "Time/Date.h"
#include "Time/Actual365.h"
#include "Process/GBMProcess.h"
#include "Payoff.h"
#include "Exercise.h"
#include "Settings.h"
#include "Math/LinearInterpolation.h"
#include "TermStructure/InterpolatedZeroCurve.h"
#include "TermStructure/BlackVarianceCurve.h"
#include "TimeGrid.h"
#include "Process/GeometricBrownianProcess.h"
#include "MonteCarlo/MultiPath.h"
#include "MonteCarlo/MultiPathGenerator.h"
#include "Math/mt19937NormalRng.h"
#include "MonteCarlo/RandomSequenceGenerator.h"
#include "Math/GeneralStatistics.h"

using std::cout;
using std::endl;
using namespace JSLib;

void plainVanillaEx();
void dayCountEx();
void linearInterpolationEx();
void zeroCurveEx();
void varianceCurveEx();
void timeGridEx();
void gbmEx();
void multiPathEx();
void statisticsEx();

int main() {
	statisticsEx();
	return 0;
}

void plainVanillaEx() {
	// plain vanilla call option pricing example
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
	cout << endl;
}

void dayCountEx() {
	// Day count convention example
	Actual365 dayCounter{};
	cout << dayCounter.yearFraction(Date(2019, 11, 1), Date(2019, 11, 10)) << endl;
	cout << endl;
}

void linearInterpolationEx() {
	// Linear interpolation example
	std::vector<double> dates = { 0.1, 0.2, 0.3, 0.5 };
	std::vector<double> rates = { 0.015, 0.02, 0.022, 0.025 };
	LinearInterpolation interpolation(dates.begin(), dates.end(), rates.begin());
	cout << interpolation(0.4) << endl;
	//cout << interpolation(0.51) << endl;	// error. interpolation does not allow extrapolation
	cout << endl;
}

void zeroCurveEx() {
	// InterpolatedZeroCurve example
	std::vector<Date> dates = { Date(2020,1,30), Date(2020,2,30),
	Date(2020, 4, 15), Date(2020,8,10) };
	std::vector<double> yields = { 0.02, 0.03, 0.032, 0.04 };
	// object slicing은 위험하다고 알고 있는데 이런 코드를 만드는게 잘하는 일인가?
	InterpolatedZeroCurve<Linear> yieldStructure(dates, yields, Actual365());
	cout << yieldStructure.zeroRate(Date(2020, 8, 9), Actual365(), Continuous) << endl;
}

void varianceCurveEx() {
	// BlackVarianceCurve example
	std::vector<Date> dates = { Date(2020,1,30), Date(2020,2,30),
	Date(2020, 4, 15), Date(2020,8,10) };
	Date refDate = dates[0];
	dates.erase(dates.begin());
	std::vector<double> vols = { 0.2, 0.18, 0.16 };
	BlackVarianceCurve volStructure(refDate, dates, vols, Actual365());
	cout << volStructure.blackVol(Date(2020, 8, 9), 100) << endl;
}

void timeGridEx() {
	// 첫번째 인수를 double type으로 넣어야 원하는 생성자가 호출됨
	TimeGrid grid = TimeGrid(10, 4);	 
	cout << grid[1] << endl;
	cout << grid.size() << endl;
}

void gbmEx() {
	GeometricBrownianMotionProcess process(0, 0.02, 0.2);
	cout << process.x0() << endl;
	cout << process.drift(0, 100) << endl; // mu*S
	cout << process.diffusion(0, 100) << endl; // sigma*S
	cout << process.expectation(0, 100, 0.01) << endl; // S0+mu*S*dt
	cout << process.stdDeviation(0, 100, 0.01) << endl; // sigma*S*dt*dt
	cout << process.evolve(0, 100, 0.01, 1) << endl;
	
}

void multiPathEx() {
	mt19937NormalRng rng;
	RandomSequenceGenerator<mt19937NormalRng> generator(12, rng);
	std::shared_ptr<GeometricBrownianMotionProcess> process = 
		std::make_shared<GeometricBrownianMotionProcess>(100, 0.02, 0.2);
	TimeGrid timeGrid(1.0, 12);
	MultiPathGenerator<RandomSequenceGenerator<mt19937NormalRng>>
		pathGenerator(process, timeGrid, generator, false);
	typedef MultiPathGenerator<
		RandomSequenceGenerator<mt19937NormalRng>>::sample_type
		sample;
	sample s = pathGenerator.next();
	cout << s.value.pathSize() << endl;
	for (int i = 0; i < s.value.pathSize(); ++i) {
		cout << s.value[0][i] << " ";
	}
	cout << endl;
}

void statisticsEx() {
	GeneralStatistics statistics;
	for (int i = 1; i <= 10; ++i) {
		statistics.add(i);
	}
	cout << statistics.mean() << endl;
}