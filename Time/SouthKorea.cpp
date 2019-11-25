#include "SouthKorea.h"

namespace JSLib {

	bool SouthKorea::SettlementImpl::isBusinessDay(const Date& date) const {
		int d = date.day();
		int m = date.month();
		int y = date.year();
		Weekday w = date.weekday();
		if (isWeekend(w)) {	// more holidays
			return false;
		}
		return true;
	}

}