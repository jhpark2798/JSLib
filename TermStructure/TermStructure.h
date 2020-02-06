#pragma once
#include "..//DesignPattern/Observer.h"
#include "..//Time/Date.h"
#include "..//Time/DayCounter.h"
#include "..//Time/Calendar.h"
#include "..//Math/Extrapolator.h"

namespace JSLib {

	class TermStructure : public virtual Observer, 
		public virtual Observable, public Extrapolator {	// �� Extrapolator�� �׳� ���?
	public:
		// Calendar, DayCounter�� default ���� �� �ʿ��ұ�?
		// �ڽ�Ŭ�������� �� �����ڸ� ȣ���Ѵٸ� referenceDate�� ���� �� ����ؾ� ��
		TermStructure(const DayCounter& dc = DayCounter());
		TermStructure(const Date& referenceDate,
			const Calendar& calendar = Calendar(),
			const DayCounter& dc = DayCounter());
		TermStructure(int settlementDays,
			const Calendar& calendar, const DayCounter& dc = DayCounter());
		virtual ~TermStructure() {}

		virtual DayCounter dayCounter() const { return dayCounter_; }
		virtual Date maxDate() const = 0; // �� pure virtual?
		virtual double maxTime() const; // �� virtual?
		virtual const Date& referenceDate() const;
		virtual Calendar calendar() const { return calendar_; }
		virtual int settlementDays() const { return settlementDays_; }
		double timeFromReference(const Date& date) const; // �����Լ�?
		void update(); // Observer::update() ����

	protected:
		void checkRange(const Date&, bool extrapolate) const;	// �����Լ�?
		void checkRange(double time, bool extrapolate) const;	// �����Լ�?

		bool moving_; // ���� date�� �ٲ�� referenceDate�� �ٲ��� �ϴ°�?
		mutable bool updated_; // �ٸ� ����������� ���� �����ؾ� �ϴ°�?
		Calendar calendar_; // �� ��� protected?

	private:
		mutable Date referenceDate_;
		int settlementDays_;	// ������ ����?
		DayCounter dayCounter_;
	};

}