#pragma once

#include "BlackVolTermStructure.h"
#include "..//Time/DayCounter.h"
#include "..//Math/Interpolation.h"

#include <vector>

namespace JSLib {
	
	// 이 클래스는 이상한 점이 한두개가 아닌데, 사실 vol surface를 많이 쓰지
	// vol curve는 잘 안쓰기 때문에 별 신경 안쓰는 것으로 보임.
	// QuantLib에서는 생성자 인풋으로 calendar를 안받음. 그리고 DayCounter 변수를
	// 클래스 안에 다시 만들어서 BlackVarianceTermStructure 에는
	// referenceDate만 넘기는 식으로 짜놨음. 나는 왜 이렇게 짰는지 이해가 안되고
	// 좋은 방법이라고 생각이 안되서 일단 평소에 짜던대로 짜놨음.
	// 그리고 ZeroCurve와 달리 클래스 템플릿이 아니라 그냥 클래스인데,
	// 그 이유는 ZeroCurve는 Interpolator를 클래스 내에 보관해야 하는 반면
	// VarianceCurve는 setInterpolation 함수 밖에서는 Interpolator의 타입을
	// 알 필요가 없기 때문에 클래스가 템플릿일 필요는 없고 setInterpolation 함수만
	// 템플릿이면 된다고 함. 이 말도 이해가 안되는게, ZeroCurve에서
	// Interpolator를 보관해야 하는 이유는 클래스가 복사될 때 Interpolation은
	// 클래스 내부 데이터의 iterator를 가지고 있기 때문에 바로 복사가 되면 안되어서
	// 따로 처리를 해주려고 Interpolator를 두는 거였음. 근데 이 문제는 
	// VarianceCurve에도 똑같이 해당되는 게 아닌가? 그러면 Interpolator를
	// 클래스 내부에 보관해야 하는게 맞아 보이는데, 왜 안할까?
	// 그리고 BlackVarianceCurve는 BlackVarianceTermStructure와
	// is a 관계가 아닌듯 보인다. BlackVarianceTermStructure는
	// strike, time 2차원의 surface니까. public 상속이 잘못된 것 같음.

	class BlackVarianceCurve : public BlackVarianceTermStructure {
	public:
		// ZeroCurve는 dates[0]을 referenceDate이라고 생각하고
		// 생성자 input에 referenceDate이 아예 없는데, 왜 VarianceCurve는 이렇게 만들었을까?
		// 왜 ZeroCurve랑 BlackVarianceCurve랑 굳이 인터페이스를 다르게 만들어서
		// 제대로 쓰기 어렵게 만들까?
		BlackVarianceCurve(const Date& referenceDate,
			const std::vector<Date>& dates, const std::vector<double>& blackVolCurve,
			const DayCounter& dc = DayCounter(), const Calendar& cal = Calendar());

		Date maxDate() const;	// TermStructure Interface
		double minStrike() const;	// VolatilityTermStructure Interface
		double maxStrike() const;	// VolatilityTermStructure Interface

		template <class Interpolator>
		void setInterpolation(const Interpolator& i = Interpolator()) {
			varianceCurve_ = i.interpolate(times_.begin(), times_end(), variances_.begin());
			varianceCurve_.update();
			notifyObservers();
		}
	protected:
		virtual double blackVarianceImpl(double t, double) const;
	private:
		Date maxDate_;
		std::vector<double> times_;
		std::vector<double> variances_;
		Interpolation varianceCurve_;
	};

}