#pragma once

#include "BlackVolTermStructure.h"
#include "..//Time/DayCounter.h"
#include "..//Math/Interpolation.h"

#include <vector>

namespace JSLib {
	
	// �� Ŭ������ �̻��� ���� �ѵΰ��� �ƴѵ�, ��� vol surface�� ���� ����
	// vol curve�� �� �Ⱦ��� ������ �� �Ű� �Ⱦ��� ������ ����.
	// QuantLib������ ������ ��ǲ���� calendar�� �ȹ���. �׸��� DayCounter ������
	// Ŭ���� �ȿ� �ٽ� ���� BlackVarianceTermStructure ����
	// referenceDate�� �ѱ�� ������ ¥����. ���� �� �̷��� ®���� ���ذ� �ȵǰ�
	// ���� ����̶�� ������ �ȵǼ� �ϴ� ��ҿ� ¥����� ¥����.
	// �׸��� ZeroCurve�� �޸� Ŭ���� ���ø��� �ƴ϶� �׳� Ŭ�����ε�,
	// �� ������ ZeroCurve�� Interpolator�� Ŭ���� ���� �����ؾ� �ϴ� �ݸ�
	// VarianceCurve�� setInterpolation �Լ� �ۿ����� Interpolator�� Ÿ����
	// �� �ʿ䰡 ���� ������ Ŭ������ ���ø��� �ʿ�� ���� setInterpolation �Լ���
	// ���ø��̸� �ȴٰ� ��. �� ���� ���ذ� �ȵǴ°�, ZeroCurve����
	// Interpolator�� �����ؾ� �ϴ� ������ Ŭ������ ����� �� Interpolation��
	// Ŭ���� ���� �������� iterator�� ������ �ֱ� ������ �ٷ� ���簡 �Ǹ� �ȵǾ
	// ���� ó���� ���ַ��� Interpolator�� �δ� �ſ���. �ٵ� �� ������ 
	// VarianceCurve���� �Ȱ��� �ش�Ǵ� �� �ƴѰ�? �׷��� Interpolator��
	// Ŭ���� ���ο� �����ؾ� �ϴ°� �¾� ���̴µ�, �� ���ұ�?
	// �׸��� BlackVarianceCurve�� BlackVarianceTermStructure��
	// is a ���谡 �ƴѵ� ���δ�. BlackVarianceTermStructure��
	// strike, time 2������ surface�ϱ�. public ����� �߸��� �� ����.

	class BlackVarianceCurve : public BlackVarianceTermStructure {
	public:
		// ZeroCurve�� dates[0]�� referenceDate�̶�� �����ϰ�
		// ������ input�� referenceDate�� �ƿ� ���µ�, �� VarianceCurve�� �̷��� ���������?
		// �� ZeroCurve�� BlackVarianceCurve�� ���� �������̽��� �ٸ��� ����
		// ����� ���� ��ư� �����?
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