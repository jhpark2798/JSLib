#pragma once
#include <string>
#include <vector>

namespace JSLib {
	
	class Date {
	public:
		Date() {}
		Date(int y, int m, int d)
			: y_(y), m_(m), d_(d), monthsThirtyOneDays({ 1,3,5,7,8,10,12 }) {	}
		Date(std::string ymd);

		int year() const { return y_; }
		int month() const { return m_; }
		int day() const { return d_; }
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