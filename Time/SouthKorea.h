#pragma once

#include <string>
#include "Calendar.h"
#include "Date.h"

namespace JSLib {

	class SouthKorea : public Calendar {
	private:
		class SettlementImpl : public Calendar::Impl {
		public:
			std::string name() const { return "South-Korea settlement"; }
			bool isBusinessDay(const Date& d) const;
			bool isWeekend(Weekday w) const { return w == Sat || w == Sun; }
		};
	};

}