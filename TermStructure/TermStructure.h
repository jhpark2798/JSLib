#pragma once
#include "..//DesignPattern/Observer.h"
#include "..//Time/Date.h"
#include "..//Time/DayCounter.h"
#include "..//Time/Calendar.h"

namespace JSLib {

	class TermStructure : public virtual Observer, public virtual Observable {
	public:
		// default ���� �ʿ��� ����? �����ڰ� 3���� �ʿ��� ����?
		// �ڽ�Ŭ�������� �� �����ڸ� ȣ���Ѵٸ� referenceDate�� ���� �� ����ؾ� ��
		TermStructure(const DayCounter& dc = DayCounter());
		TermStructure(const Date& referenceDate,
			const Calendar& calendar = Calendar(),
			const DayCounter& dc = DayCounter());
		TermStructure(int settlementDays,
			const Calendar& calendar, const DayCounter& dc = DayCounter());
		virtual ~TermStructure() {}

		virtual DayCounter dayCounter() const { return dayCounter_; }
		virtual Date maxDate() const = 0; // �����Լ�?
		virtual double maxTime() const; // �����Լ�?
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