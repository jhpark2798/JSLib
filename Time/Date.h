#pragma once
#include <string>
#include <vector>

namespace JSLib {

	enum Weekday {
		Wed = 0,
		Thu,
		Fri,
		Sat,
		Sun,
		Mon,
		Tue,
	};
	
	class Date {
	public:
		Date() {}
		Date(int y, int m, int d)
			: y_(y), m_(m), d_(d), monthsThirtyOneDays{ 1,3,5,7,8,10,12 } {
			//monthsThirtyOneDays.push_back(1);
			//monthsThirtyOneDays.push_back(3);
			//monthsThirtyOneDays.push_back(5);
			//monthsThirtyOneDays.push_back(7);
			//monthsThirtyOneDays.push_back(8);
			//monthsThirtyOneDays.push_back(10);
			//monthsThirtyOneDays.push_back(12);
		}
		Date(std::string ymd);

		int year() const { return y_; }
		int month() const { return m_; }
		int day() const { return d_; }
		Weekday weekday() const;
		int daysFrom(const Date& d) const;
		Date addDays(int days);

		void print();
		std::string to_str();
		bool isLeapYear();

		Date& operator++();
		Date operator++(int);
		Date operator+(int days);
		bool operator>(Date& rhs) const;
		bool operator>(const Date& rhs) const;
		bool operator<(Date& rhs) const;
		bool operator<(const Date& rhs) const;
		bool operator>=(Date& rhs) const;
		bool operator>=(const Date& rhs) const;
		bool operator<=(Date& rhs) const;
		bool operator<=(const Date& rhs) const;
		bool operator==(const Date& rhs) const;
		bool operator==(Date& rhs) const;
		bool operator!=(const Date& rhs) const;
		bool operator!=(Date& rhs) const;

	private:
		int y_, m_, d_;
		std::vector<int> monthsThirtyOneDays;
	};

	int daysBetween(const Date& d1, const Date& d2);

}