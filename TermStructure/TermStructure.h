#pragma once
#include "..//DesignPattern/Observer.h"
#include "..//Time/Date.h"
#include "..//Time/DayCounter.h"
#include "..//Time/Calendar.h"

namespace JSLib {

	class TermStructure : public virtual Observer, public virtual Observable {
	public:
		// default 값이 필요한 이유? 생성자가 3개나 필요한 이유?
		// 자식클래스에서 이 생성자를 호출한다면 referenceDate가 없는 걸 고려해야 함
		TermStructure(const DayCounter& dc = DayCounter());
		TermStructure(const Date& referenceDate,
			const Calendar& calendar = Calendar(),
			const DayCounter& dc = DayCounter());
		TermStructure(int settlementDays,
			const Calendar& calendar, const DayCounter& dc = DayCounter());
		virtual ~TermStructure() {}

		virtual DayCounter dayCounter() const { return dayCounter_; }
		virtual Date maxDate() const = 0; // 무슨함수?
		virtual double maxTime() const; // 무슨함수?
		virtual const Date& referenceDate() const;
		virtual Calendar calendar() const { return calendar_; }
		virtual int settlementDays() const { return settlementDays_; }
		double timeFromReference(const Date& date) const; // 무슨함수?
		void update(); // Observer::update() 구현

	protected:
		void checkRange(const Date&, bool extrapolate) const;	// 무슨함수?
		void checkRange(double time, bool extrapolate) const;	// 무슨함수?


		bool moving_; // 오늘 date이 바뀌면 referenceDate도 바뀌어야 하는가?
		mutable bool updated_; // 다른 멤버변수들을 새로 갱신해야 하는가?
		Calendar calendar_; // 왜 얘는 protected?

	private:
		mutable Date referenceDate_;
		int settlementDays_;	// 변수의 역할?
		DayCounter dayCounter_;
	};

}