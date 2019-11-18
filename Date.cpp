#include "Date.h"
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>

namespace JSLib {

	Date::Date(std::string ymd) {
		y_ = std::atoi(ymd.substr(0, 4).c_str());
		m_ = std::atoi(ymd.substr(4, 2).c_str());
		d_ = std::atoi(ymd.substr(6, 2).c_str());
	}

	int Date::daysFrom(const Date& d) const {
		std::tm a = { 0, 0, 0, d_, m_ - 1, y_ - 1900 };
		std::tm b = { 0, 0, 0, d.day(), d.month() - 1, d.year() - 1900 };
		std::time_t x = std::mktime(&a);
		std::time_t y = std::mktime(&b);
		int difference = std::difftime(y, x) / (60 * 60 * 24);
		return difference;
	}

	Date Date::addDays(int days) {
		for (int i = 0; i < days; ++i) ++(*this);
		return *this;
	}

	bool Date::operator>(const Date& rhs) const {
		bool res = false;
		if (y_ != rhs.y_)	 res = y_ > rhs.y_ ? true : false;
		else if (m_ != rhs.m_) res = m_ > rhs.m_ ? true : false;
		else if (d_ != rhs.d_) res = d_ > rhs.d_ ? true : false;
		return res;
	}

	bool Date::operator>(Date& rhs) const {
		return operator>(static_cast<const Date&>(rhs));
	}

	bool Date::operator<(Date& rhs) const {
		return rhs > * this;
	}

	bool Date::operator<(const Date& rhs) const {
		return rhs > * this;
	}

	bool Date::operator>=(const Date& rhs) const {
		return !operator<(rhs);
	}

	bool Date::operator>=(Date& rhs) const {
		return !operator<(rhs);
	}

	bool Date::operator<=(const Date& rhs) const {
		return !operator>(rhs);
	}

	bool Date::operator<=(Date& rhs) const {
		return !operator>(rhs);
	}

	void Date::print() {
		std::cout << y_ << "/" << m_ << "/" << d_ << std::endl;
	}

	std::string Date::to_str() {
		std::stringstream ss;
		ss.str("");
		ss.clear();
		ss << y_ << "/" << m_ << "/" << d_;
		return ss.str();
	}

	bool Date::isLeapYear() {
		if (y_ % 400 == 0) return true;
		if (y_ % 4 == 0 && m_ % 100 != 0) return true;
		return false;
	}

	Date& Date::operator++() {
		if (d_ == 31) {
			d_ = 1;
			m_++;
		}
		else if (d_ == 30 &&
			std::find(monthsThirtyOneDays.begin(), monthsThirtyOneDays.end(), m_)
			== monthsThirtyOneDays.end()) {
			d_ = 1;
			m_++;
		}
		else if (d_ == 29 && m_ == 2) {
			d_ = 1;
			m_++;
		}
		else if (d_ == 28 && m_ == 2 && !isLeapYear()) {
			d_ = 1;
			m_++;
		}
		else
			d_++;
		if (m_ > 12) {
			m_ = 1;
			y_++;
		}
		return *this;
	}

	Date Date::operator++(int) {
		Date res(*this);
		++(*this);
		return res;
	}

	Date Date::operator+(int days) {
		Date res(*this);
		res.addDays(days);
		return res;
	}

	bool Date::operator==(const Date& rhs) const {
		return y_ != rhs.y_ || m_ != rhs.m_ || d_ != rhs.d_ ? false : true;
	}

	bool Date::operator==(Date& rhs) const {
		return operator==(static_cast<const Date&>(rhs));
	}

	bool Date::operator!=(const Date& rhs) const {
		return !operator==(rhs);
	}

	bool Date::operator!=(Date& rhs) const {
		return !operator==(rhs);
	}

	int daysBetween(const Date& d1, const Date& d2) {
		return d1.daysFrom(d2);
	}

}